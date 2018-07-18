open AssetNodeType;

open DiffType;

type state = {style: ReactDOMRe.Style.t};

type retainedProps = {map: option(int)};

type action =
  | Nothing
  | DragEnter
  | DragLeave
  | DragDrop(int);

module Method = {
  let isFlag = startFlag =>
    switch (startFlag) {
    | None => false
    | Some(flag) => flag == AssetUtils.getFlag()
    };

  let isTypeValid = (startId, assetState) =>
    switch (startId) {
    | None => false
    | Some(id) =>
      assetState
      |> TextureNodeMapAssetService.getTextureNodeMap
      |> WonderCommonlib.SparseMapService.get(id)
      |> Js.Option.isSome
    };

  let onDrop = MaterialDragTextureEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState;

  let removeTexture = ((store, dispatchFunc), (), materialComponent) =>
    switch (
      BasicMaterialEngineService.getMap(materialComponent)
      |> StateLogicService.getEngineStateToGetData
    ) {
    | None => ()
    | Some(_mapId) =>
      MaterialRemoveTextureEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState(
        (store, dispatchFunc),
        (),
        materialComponent,
      )
    };

  let _isTriggerAction = (isFlagFunc, isTypeValidFunc) => {
    let (flag, startId) =
      StateEditorService.getState()
      |> CurrentDragSourceEditorService.getCurrentDragSource;

    isFlagFunc(flag)
    && isTypeValidFunc(startId, StateAssetService.getState());
  };

  let handleDragEnter = (isFlagFunc, isTypeValidFunc, _event) =>
    _isTriggerAction(isFlagFunc, isTypeValidFunc) ? DragEnter : Nothing;

  let handleDragLeave = (isFlagFunc, isTypeValidFunc, event) => {
    ReactEventType.convertReactMouseEventToJsEvent(event)
    |> DomHelper.stopPropagation;

    _isTriggerAction(isFlagFunc, isTypeValidFunc) ? DragLeave : Nothing;
  };

  let handleDragOver = event =>
    ReactEventType.convertReactMouseEventToJsEvent(event)
    |> DomHelper.preventDefault;

  let handleDrop = (isFlagFunc, isTypeValidFunc, event) => {
    let startId =
      ReactEventType.convertReactMouseEventToJsEvent(event)
      |> DragUtils.getDragedUid;

    _isTriggerAction(isFlagFunc, isTypeValidFunc) ?
      DragDrop(startId) : DragLeave;
  };
};

let component =
  ReasonReact.reducerComponentWithRetainedProps("MainEditorBasicMaterialMap");

let reducer = ((store, dispatchFunc), materialComponent, action, state) =>
  switch (action) {
  | DragEnter =>
    ReasonReact.Update({
      ...state,
      style:
        ReactUtils.addStyleProp("border", "2px dashed blue", state.style),
    })

  | DragLeave =>
    ReasonReact.Update({
      ...state,
      style: ReactUtils.addStyleProp("border", "1px solid red", state.style),
    })

  | DragDrop(startId) =>
    ReasonReactUtils.sideEffects(() =>
      Method.onDrop((store, dispatchFunc), materialComponent, startId)
    )

  | Nothing => ReasonReact.NoUpdate
  };

let render =
    (
      (store, dispatchFunc),
      materialComponent,
      {state, retainedProps, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <article className="wonder-material-texture">
    <div
      style=state.style
      className="texture_ground"
      onDragEnter=(
        _e =>
          send(Method.handleDragEnter(Method.isFlag, Method.isTypeValid, _e))
      )
      onDragLeave=(
        _e =>
          send(Method.handleDragLeave(Method.isFlag, Method.isTypeValid, _e))
      )
      onDragOver=Method.handleDragOver
      onDrop=(
        _e => send(Method.handleDrop(Method.isFlag, Method.isTypeValid, _e))
      )
    />
    <span className=""> (DomHelper.textEl("texture:")) </span>
    (
      switch (retainedProps.map) {
      | None => <img src="./public/img/null.jpg" />
      | Some(map) =>
        <img
          src=(
            StateAssetService.getState()
            |> ImageBase64MapAssetService.getImageBase64Map
            |> WonderCommonlib.SparseMapService.unsafeGet(map)
          )
        />
      }
    )
    <button
      className="texture_remove"
      onClick=(
        e =>
          Method.removeTexture((store, dispatchFunc), (), materialComponent)
      )>
      (DomHelper.textEl("remove"))
    </button>
  </article>;

let shouldUpdate =
    ({oldSelf, newSelf}: ReasonReact.oldNewSelf('a, retainedProps, 'c)) =>
  oldSelf.retainedProps != newSelf.retainedProps
  || oldSelf.state != newSelf.state;

let make =
    (~store: AppStore.appState, ~dispatchFunc, ~materialComponent, _children) => {
  ...component,
  retainedProps: {
    map:
      BasicMaterialEngineService.getMap(materialComponent)
      |> StateLogicService.getEngineStateToGetData,
  },
  initialState: () => {style: ReactDOMRe.Style.make(~opacity="1", ())},
  reducer: reducer((store, dispatchFunc), materialComponent),
  shouldUpdate,
  render: self => render((store, dispatchFunc), materialComponent, self),
};