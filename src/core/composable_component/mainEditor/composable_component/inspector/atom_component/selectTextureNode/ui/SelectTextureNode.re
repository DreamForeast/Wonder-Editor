open NodeAssetType;

type state = {
  style: ReactDOMRe.Style.t,
  isShowTextureGroup: bool,
  currentTextureComponent: option(int),
};

type retainedProps = {map: option(int)};

type action =
  | Nothing
  | DragEnter
  | DragLeave
  | SetTextureToEngine(int, int)
  | ShowTextureGroup
  | HideTextureGroup;

module Method = {
  let isWidget = startWidget =>
    switch (startWidget) {
    | None => false
    | Some(widget) => widget == AssetWidgetService.getWidget()
    };

  let isTypeValid = (startNodeId, editorState) =>
    switch (startNodeId) {
    | None => false
    | Some(id) =>
      OperateTreeAssetEditorService.isNodeExistById(id, editorState)
    };

  let _isTriggerAction = (isWidgetFunc, isTypeValidFunc) => {
    let (widget, startNodeId) =
      StateEditorService.getState()
      |> CurrentDragSourceEditorService.getCurrentDragSource;

    isWidgetFunc(widget)
    && isTypeValidFunc(startNodeId, StateEditorService.getState());
  };

  let handleDragEnter = (isWidgetFunc, isTypeValidFunc, _event) =>
    _isTriggerAction(isWidgetFunc, isTypeValidFunc) ? DragEnter : Nothing;

  let handleDragLeave = (isWidgetFunc, isTypeValidFunc, event) => {
    ReactEventType.convertReactMouseEventToJsEvent(event)
    |> EventHelper.stopPropagation;

    _isTriggerAction(isWidgetFunc, isTypeValidFunc) ? DragLeave : Nothing;
  };

  let handleDragOver = event =>
    ReactEventType.convertReactMouseEventToJsEvent(event)
    |> EventHelper.preventDefault;

  let handleDragDrop = (isWidgetFunc, isTypeValidFunc, event) => {
    let e = ReactEventType.convertReactMouseEventToJsEvent(event);
    let startNodeId = e |> DragUtils.getDragedId;

    EventHelper.preventDefault(e);

    _isTriggerAction(isWidgetFunc, isTypeValidFunc) ?
      SetTextureToEngine(
        startNodeId,
        OperateTreeAssetEditorService.unsafeFindNodeById(startNodeId)
        |> StateLogicService.getEditorState
        |> TextureNodeAssetService.getTextureComponent,
      ) :
      DragLeave;
  };

  let showMapComponent = currentTextureComponent =>
    switch (currentTextureComponent) {
    | None => ReasonReact.null
    | Some(map) =>
      let source =
        BasicSourceTextureEngineService.unsafeGetSource(
          map,
          StateEngineService.unsafeGetState(),
        );

      <img
        src=ImageType.convertImageElementToSrcImageElements(source)##src
      />;
    };

  let _sortByName = (engineState, allTextureNodes) =>
    allTextureNodes
    |> Js.Array.sortInPlaceWith((textureNode1, textureNode2) =>
         Js.String.localeCompare(
           NodeNameAssetLogicService.getTextureNodeName(
             ~texture=
               TextureNodeAssetService.getTextureComponent(textureNode2),
             ~engineState,
           )
           |> Js.String.charAt(0),
           NodeNameAssetLogicService.getTextureNodeName(
             ~texture=
               TextureNodeAssetService.getTextureComponent(textureNode1),
             ~engineState,
           )
           |> Js.String.charAt(0),
         )
         |> NumberType.convertFloatToInt
       );

  let _buildTextureUIComponent =
      ((className, nodeId, textureComponent, imgSrc), sendFunc, engineState) =>
    <div
      className
      key={DomHelper.getRandomKey()}
      onClick={_e => sendFunc(SetTextureToEngine(nodeId, textureComponent))}>
      <img src=imgSrc className="imgContent-img" />
      <div className="imgContent-text">
        {
          DomHelper.textEl(
            NodeNameAssetLogicService.getTextureNodeName(
              ~texture=textureComponent,
              ~engineState,
            ),
          )
        }
      </div>
    </div>;

  let _buildTextureUIComponentResult =
      (
        (nodeId, textureComponent, imageDataIndex, className),
        sendFunc,
        (editorState, engineState),
      ) => {
    let imgSrc = ImageDataMapUtils.getImgSrc(imageDataIndex, editorState);

    _buildTextureUIComponent(
      (className, nodeId, textureComponent, imgSrc),
      sendFunc,
      engineState,
    )
    |> Result.SameDataResult.success;
  };

  let showTextureAssets = (state, sendFunc, findAllTextureNodesFunc) => {
    let editorState = StateEditorService.getState();
    let engineState = StateEngineService.unsafeGetState();

    /* BasicSourceTypeTextureNodeAssetEditorService.findAllBasicSourceTypeTextureNodes(
         editorState,
       ) */
    findAllTextureNodesFunc(editorState)
    |> _sortByName(engineState)
    |> ArrayService.traverseSameDataResultAndCollectByApply(textureNode => {
         let {textureComponent, imageDataIndex}: NodeAssetType.textureNodeData =
           TextureNodeAssetService.getNodeData(textureNode);
         let nodeId = NodeAssetService.getNodeId(~node=textureNode);

         switch (state.currentTextureComponent) {
         | None =>
           _buildTextureUIComponentResult(
             (
               nodeId,
               textureComponent,
               imageDataIndex,
               "select-item-imgContent",
             ),
             sendFunc,
             (editorState, engineState),
           )
         | Some(map) =>
           let className =
             map === textureComponent ?
               "select-item-imgContent select-item-active" :
               "select-item-imgContent";

           _buildTextureUIComponentResult(
             (nodeId, textureComponent, imageDataIndex, className),
             sendFunc,
             (editorState, engineState),
           );
         };
       });
  };
};

let component = ReasonReact.reducerComponent("MainEditorMaterialMap");

let _handleSetTextureToEngine =
    ((textureNodeId, textureComponent), onDropFunc, state) =>
  switch (state.currentTextureComponent) {
  | None =>
    ReasonReactUtils.updateWithSideEffects(
      {...state, currentTextureComponent: Some(textureComponent)}, _state =>
      onDropFunc(textureNodeId)
    )
  | Some(sourceTextureComponent) =>
    sourceTextureComponent === textureComponent ?
      ReasonReact.NoUpdate :
      ReasonReactUtils.updateWithSideEffects(
        {...state, currentTextureComponent: Some(textureComponent)}, _state =>
        onDropFunc(textureNodeId)
      )
  };

let reducer = (onDropFunc, action, state) =>
  switch (action) {
  | DragEnter =>
    ReasonReact.Update({
      ...state,
      style:
        ReactUtils.addStyleProp("border", "2px solid coral", state.style),
    })
  | DragLeave =>
    ReasonReact.Update({
      ...state,
      style:
        ReactUtils.addStyleProp(
          "border",
          "2px solid rgb(167,165,165)",
          state.style,
        ),
    })
  | SetTextureToEngine(textureNodeId, textureComponent) =>
    _handleSetTextureToEngine(
      (textureNodeId, textureComponent),
      onDropFunc,
      state,
    )
  | Nothing => ReasonReact.NoUpdate
  | ShowTextureGroup =>
    ReasonReact.Update({...state, isShowTextureGroup: true})
  | HideTextureGroup =>
    ReasonReact.Update({...state, isShowTextureGroup: false})
  };

let _renderDragableImage = ({state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <div
    className="texture-img"
    onClick={_e => send(ShowTextureGroup)}
    style={state.style}>
    <div
      className="img-dragBg"
      onDragEnter={
        _e =>
          send(
            Method.handleDragEnter(Method.isWidget, Method.isTypeValid, _e),
          )
      }
      onDragLeave={
        _e =>
          send(
            Method.handleDragLeave(Method.isWidget, Method.isTypeValid, _e),
          )
      }
      onDragOver=Method.handleDragOver
      onDrop={
        _e =>
          send(
            Method.handleDragDrop(Method.isWidget, Method.isTypeValid, _e),
          )
      }
    />
    {Method.showMapComponent(state.currentTextureComponent)}
  </div>;

/* TODO change texture not close group */
let _renderTextureGroup = (state, sendFunc, findAllTextureNodesFunc) =>
  <div className="select-component-content">
    <div className="select-component-item">
      <div className="select-item-header">
        {DomHelper.textEl("Texture")}
      </div>
      <div className="select-item-imgBody">
        <div className="imgBody-content">
          {
            ReasonReact.array(
              Method.showTextureAssets(
                state,
                sendFunc,
                findAllTextureNodesFunc,
              )
              |> ResultUtils.handleError,
            )
          }
        </div>
      </div>
    </div>
    <div
      className="select-component-bg"
      onClick={_e => sendFunc(HideTextureGroup)}
    />
  </div>;

let render =
    (
      (label, title),
      (removeTextureFunc, findAllTextureNodesFunc),
      ({state, send}: ReasonReact.self('a, 'b, 'c)) as self,
    ) => {
  let languageType =
    LanguageEditorService.unsafeGetType |> StateLogicService.getEditorState;

  <article className="inspector-item">
    <div
      className="item-header"
      title={
        switch (title) {
        | None => ""
        | Some(title) => title
        }
      }>
      {DomHelper.textEl(label)}
    </div>
    <div className="item-content item-texture">
      {_renderDragableImage(self)}
      <button className="texture-remove" onClick={e => removeTextureFunc()}>
        {DomHelper.textEl("Remove")}
      </button>
    </div>
    {
      state.isShowTextureGroup ?
        _renderTextureGroup(state, send, findAllTextureNodesFunc) :
        ReasonReact.null
    }
  </article>;
};

let make =
    (
      ~label,
      ~currentTextureComponent,
      ~onDropFunc,
      ~removeTextureFunc,
      ~findAllTextureNodesFunc,
      ~isShowTextureGroup,
      ~title: option(string)=?,
      _children,
    ) => {
  ...component,
  initialState: () => {
    style: ReactDOMRe.Style.make(~opacity="1", ()),
    isShowTextureGroup,
    currentTextureComponent,
  },
  reducer: reducer(onDropFunc),
  render: self =>
    render(
      (label, title),
      (removeTextureFunc, findAllTextureNodesFunc),
      self,
    ),
};