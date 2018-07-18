let _getFromArray = (array, index) => ArrayService.getNth(index, array);

let _getTriggerRenameInput = domChildren => {
  let article = _getFromArray(domChildren, 0);
  let nameArticle = _getFromArray(article##children, 0);
  let div = _getFromArray(nameArticle##children, 1);
  let inputArticle = _getFromArray(div##children, 0);
  let input = _getFromArray(inputArticle##children, 0);

  input;
};
let triggerRenameChangeEvent = (value, domChildren) => {
  let input = _getTriggerRenameInput(domChildren);
  BaseEventTool.triggerChangeEvent(
    input,
    BaseEventTool.buildFormEvent(value),
  );
};
let triggerRenameBlurEvent = (value, domChildren) => {
  let input = _getTriggerRenameInput(domChildren);
  BaseEventTool.triggerBlurEvent(input, BaseEventTool.buildFormEvent(value));
};