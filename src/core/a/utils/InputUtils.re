open InputType;

let changeInput = (inputRegEx, event) => {
  let inputVal =
    ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;

  switch (inputVal) {
  | "" => Change(Some(""))
  | "-" => Change(Some("-"))
  | value =>
    switch (inputRegEx |> Js.Re.test(value)) {
    | false => Change(None)
    | true => Change(Some(value))
    }
  };
};

let triggerOnChange = (value, (convertToStringFunc, onChangeFunc)) =>
  switch (onChangeFunc) {
  | None => ()
  | Some(onChange) => onChange(convertToStringFunc(value))
  };

let triggerOnBlur = (value, (convertToStringFunc, onBlurFunc)) =>
  switch (onBlurFunc) {
  | None => ()
  | Some(onBlur) => onBlur(convertToStringFunc(value))
  };

let handleDragStart = (event, dragStartType, send) => {
  let e = ReactEventType.convertReactMouseEventToJsEvent(event);

  Wonderjs.DomExtend.requestPointerLock(e##target);

  send(dragStartType);
};