open InputType;

type state = {
  inputValue: option(string),
  isDragStart: bool,
};

module Method = {
  let getIntRegEx = () => [%re {|/^-?(0|[1-9][0-9]*)$/|}];

  let handleChangeAction = (state, onChangeFunc, value) =>
    switch (value) {
    | None => ReasonReact.NoUpdate
    | Some("-") => ReasonReact.Update({...state, inputValue: Some("-")})
    | Some("") => ReasonReact.Update({...state, inputValue: None})
    | Some(value) =>
      ReasonReactUtils.updateWithSideEffects(
        {...state, inputValue: Some(value)}, _state =>
        InputUtils.triggerOnChange(value, (int_of_string, onChangeFunc))
      )
    };

  let handleBlurAction = (state, (onChangeFunc, onBlurFunc), languageType) =>
    switch (state.inputValue) {
    | None
    | Some("-")
    | Some("") =>
      let value = "0";
      ReasonReactUtils.updateWithSideEffects(
        {...state, inputValue: Some(value)},
        _state => {
          InputUtils.triggerOnChange(value, (int_of_string, onChangeFunc));
          InputUtils.triggerOnBlur(value, (int_of_string, onBlurFunc));
        },
      );
    | Some(value) =>
      ReasonReactUtils.sideEffects(_state =>
        InputUtils.triggerOnBlur(value, (int_of_string, onBlurFunc))
      )
    };

  let computeNewValue = (currentValue, (movementX, movementY)) => {
    let factor = 10;

    currentValue + movementX - movementY;
  };

  let handleDragStart = (event, send) =>
    InputUtils.handleDragStart(event, DragStart, send);

  let handleDragDrop = (event, (send, state), onDragDropFunc) =>
    state.isDragStart ?
      {
        Wonderjs.DomExtend.exitPointerLock();

        onDragDropFunc(
          state.inputValue |> OptionService.unsafeGet |> int_of_string,
        );

        send(DragDrop) |> ignore;
      } :
      ();

  let handleDragOver = (event, (send, state)) =>
    state.isDragStart ?
      {
        let e = ReactEventType.convertReactMouseEventToJsEvent(event);

        send(
          Change(
            Some(
              computeNewValue(
                state.inputValue |> OptionService.unsafeGet |> int_of_string,
                MouseEventService.getMovementDeltaWhenPointerLockedAndFixBug(
                  e,
                ),
              )
              |> string_of_int,
            ),
          ),
        )
        |> ignore;
      } :
      ();

  let renderLabel = ((send, state), label, title, onDragDropFunc) =>
    switch (label) {
    | None => ReasonReact.null
    | Some(label) =>
      <div
        className="item-header component-label"
        onMouseDown=(event => handleDragStart(event, send))
        onMouseMove=(event => handleDragOver(event, (send, state)))
        onMouseUp=(
          event => handleDragDrop(event, (send, state), onDragDropFunc)
        )
        title={
          switch (title) {
          | None => ""
          | Some(title) => title
          }
        }>
        {DomHelper.textEl(label)}
      </div>
    };

  let renderContent = ((send, state)) =>
    <div className="item-content">
      <input
        className="input-component int-input"
        type_="text"
        value={
          switch (state.inputValue) {
          | None => ""
          | Some(value) => value
          }
        }
        onChange={e => send(InputUtils.changeInput(getIntRegEx(), e))}
        onBlur={_e => send(Blur)}
      />
    </div>;
};

let component = ReasonReact.reducerComponent("IntInput");

let reducer = ((onChangeFunc, onBlurFunc), action, state) => {
  let languageType =
    LanguageEditorService.unsafeGetType |> StateLogicService.getEditorState;

  switch (action) {
  | DragStart => ReasonReact.Update({...state, isDragStart: true})
  | DragDrop => ReasonReact.Update({...state, isDragStart: false})
  | Change(value) => Method.handleChangeAction(state, onChangeFunc, value)
  | Blur =>
    Method.handleBlurAction(state, (onChangeFunc, onBlurFunc), languageType)
  };
};

let render =
    (
      label,
      title,
      (onBlurFunc, onDragDropFunc),
      {state, handle, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <article className="inspector-item wonder-int-input">
    {Method.renderLabel((send, state), label, title, onDragDropFunc)}
    {Method.renderContent((send, state))}
  </article>;

let make =
    (
      ~onDragDrop=_ => (),
      ~defaultValue: option(string)=?,
      ~label: option(string)=?,
      ~onChange: option(int => unit)=?,
      ~onBlur: option(int => unit)=?,
      ~title: option(string)=?,
      _children,
    ) => {
  ...component,
  initialState: () =>
    switch (defaultValue) {
    | None => {inputValue: Some("0"), isDragStart: false}
    | Some(value) => {inputValue: Some(value), isDragStart: false}
    },
  reducer: reducer((onChange, onBlur)),
  render: self => render(label, title, (onBlur, onDragDrop), self),
};