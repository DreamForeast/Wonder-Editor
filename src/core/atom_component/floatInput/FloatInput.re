open InputType;

type state = {
  inputValue: option(string),
  originValue: string,
  isDragStart: bool,
  canBeZero: bool,
};

module Method = {
  let getFloatRegEx = () => [%re {|/^-?(0|[1-9][0-9]*)(\.[0-9]{0,6})?$/|}];

  let handleSpecificFuncByCanBeZero =
      (state, value, canBeZero, (canBeZeroFunc, canNotBeZeroFunc)) =>
    canBeZero ? canBeZeroFunc(value) : canNotBeZeroFunc(value);

  let handleChangeAction = (state, onChangeFunc, canBeZero, value) =>
    switch (value) {
    | None => ReasonReact.NoUpdate
    | Some("-") => ReasonReact.Update({...state, inputValue: Some("-")})
    | Some("") => ReasonReact.Update({...state, inputValue: None})
    | Some("0") =>
      handleSpecificFuncByCanBeZero(
        state,
        "0",
        canBeZero,
        (
          value =>
            ReasonReactUtils.updateWithSideEffects(
              {...state, inputValue: Some(value)}, _state =>
              InputUtils.triggerOnChange(
                value,
                (float_of_string, onChangeFunc),
              )
            ),
          value => ReasonReact.Update({...state, inputValue: Some(value)}),
        ),
      )
    | Some("0.") =>
      handleSpecificFuncByCanBeZero(
        state,
        "0.",
        canBeZero,
        (
          value =>
            ReasonReactUtils.updateWithSideEffects(
              {...state, inputValue: Some(value)}, _state =>
              InputUtils.triggerOnChange(
                value,
                (float_of_string, onChangeFunc),
              )
            ),
          value => ReasonReact.Update({...state, inputValue: Some(value)}),
        ),
      )
    | Some(value) =>
      ReasonReactUtils.updateWithSideEffects(
        {...state, inputValue: Some(value)}, _state =>
        InputUtils.triggerOnChange(value, (float_of_string, onChangeFunc))
      )
    };

  let handleBlurAction =
      (state, (onChangeFunc, onBlurFunc), canBeZero, languageType) =>
    switch (state.inputValue) {
    | None
    | Some("-")
    | Some("") =>
      handleSpecificFuncByCanBeZero(
        state,
        "0",
        canBeZero,
        (
          value =>
            ReasonReactUtils.updateWithSideEffects(
              {...state, inputValue: Some(value)},
              _state => {
                InputUtils.triggerOnChange(
                  value,
                  (float_of_string, onChangeFunc),
                );
                InputUtils.triggerOnBlur(
                  value,
                  (float_of_string, onBlurFunc),
                );
              },
            ),
          value =>
            ReasonReact.Update({
              ...state,
              inputValue: Some(state.originValue),
            }),
        ),
      )
    | Some("0")
    | Some("0.") =>
      handleSpecificFuncByCanBeZero(
        state,
        "0",
        canBeZero,
        (
          value =>
            ReasonReactUtils.updateWithSideEffects(
              {...state, inputValue: Some(value)}, _state =>
              InputUtils.triggerOnBlur(value, (float_of_string, onBlurFunc))
            ),
          _value => {
            ConsoleUtils.warn("shouldn't be zero")
            |> StateLogicService.getEditorState;

            ReasonReact.Update({
              ...state,
              inputValue: Some(state.originValue),
            });
          },
        ),
      )
    | Some(value) =>
      ReasonReactUtils.updateWithSideEffects(
        {...state, originValue: value}, _state =>
        InputUtils.triggerOnBlur(value, (float_of_string, onBlurFunc))
      )
    };

  let _getReplacedZero = () => 0.001;

  let _isNearlyZero = value => value |> Js.Math.abs_float <= 0.001;

  let computeNewValue = (currentValue, canBeZero, (movementX, movementY)) => {
    let factor = 100.0;

    let newValue =
      currentValue
      +. NumberType.convertIntToFloat(movementX)
      /. factor
      -. NumberType.convertIntToFloat(movementY)
      /. factor;

    canBeZero ?
      newValue : _isNearlyZero(newValue) ? _getReplacedZero() : newValue;
  };

  let handleDragStart = (event, send) =>
    InputUtils.handleDragStart(event, DragStart, send);

  let handleDragDrop = (event, (send, state), onDragDropFunc) =>
    state.isDragStart ?
      {
        Wonderjs.DomExtend.exitPointerLock();

        onDragDropFunc(
          state.inputValue |> OptionService.unsafeGet |> float_of_string,
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
                state.inputValue |> OptionService.unsafeGet |> float_of_string,
                state.canBeZero,
                MouseEventService.getMovementDeltaWhenPointerLockedAndFixBug(
                  e,
                ),
              )
              |> string_of_float,
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
        className="input-component float-input"
        type_="text"
        value={
          switch (state.inputValue) {
          | None => ""
          | Some(value) => value
          }
        }
        onChange={e => send(InputUtils.changeInput(getFloatRegEx(), e))}
        onBlur={_e => send(Blur)}
      />
    </div>;
};

let component = ReasonReact.reducerComponent("FloatInput");

let reducer = ((onChangeFunc, onBlurFunc), canBeZero, action, state) => {
  let languageType =
    LanguageEditorService.unsafeGetType |> StateLogicService.getEditorState;

  switch (action) {
  | DragStart => ReasonReact.Update({...state, isDragStart: true})
  | DragDrop => ReasonReact.Update({...state, isDragStart: false})
  | Change(value) =>
    Method.handleChangeAction(state, onChangeFunc, canBeZero, value)
  | Blur =>
    Method.handleBlurAction(
      state,
      (onChangeFunc, onBlurFunc),
      canBeZero,
      languageType,
    )
  };
};

let render =
    (
      label,
      title,
      (onBlurFunc, onDragDropFunc),
      {state, handle, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <article className="inspector-item wonder-float-input">
    {Method.renderLabel((send, state), label, title, onDragDropFunc)}
    {Method.renderContent((send, state))}
  </article>;

let make =
    (
      ~canBeZero: bool=true,
      ~onDragDrop=_ => (),
      ~defaultValue: option(string)=?,
      ~label: option(string)=?,
      ~onChange: option(float => unit)=?,
      ~onBlur: option(float => unit)=?,
      ~title: option(string)=?,
      _children,
    ) => {
  ...component,
  initialState: () =>
    switch (defaultValue) {
    | None => {
        inputValue: Some("0"),
        originValue: "0",
        isDragStart: false,
        canBeZero,
      }
    | Some(value) => {
        inputValue: Some(value),
        originValue: value,
        isDragStart: false,
        canBeZero,
      }
    },
  reducer: reducer((onChange, onBlur), canBeZero),
  render: self => render(label, title, (onBlur, onDragDrop), self),
};