open UpdateStore;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = unit;
  type dataTuple = option(Wonderjs.GameObjectPrimitiveType.gameObject);
  type return = unit;

  let rec _setAllParentsShowChildren = (gameObject, engineState, editorState) =>
    switch (
      HierarchyGameObjectEngineService.getParentGameObject(
        gameObject,
        engineState,
      )
    ) {
    | None => editorState
    | Some(parentGameObject) =>
      _setAllParentsShowChildren(
        parentGameObject,
        engineState,
        editorState
        |> SceneTreeEditorService.setIsShowChildren(parentGameObject, true),
      )
    };

  let handleSelfLogic = ((uiState, dispatchFunc), (), gameObjectOpt) => {
    switch (gameObjectOpt) {
    | None =>
      StateEditorService.getState()
      |> SceneTreeSelectCurrentNodeUtils.clearCurrentData
      |> StateEditorService.setState
      |> ignore
    | Some(gameObject) =>
      StateEditorService.getState()
      |> SceneTreeSelectCurrentNodeUtils.clearCurrentData
      |> StateEditorService.setState
      |> ignore;

      StateEditorService.getState()
      |> SceneTreeEditorService.setCurrentSceneTreeNode(gameObject)
      |> CurrentSelectSourceEditorService.setCurrentSelectSource(
           SceneTreeWidgetService.getWidget(),
         )
      |> _setAllParentsShowChildren(
           gameObject,
           StateEngineService.unsafeGetState(),
         )
      |> StateEditorService.setState
      |> ignore;

      let engineState = StateEngineService.unsafeGetState();
      let (engineState, _) =
        ManageEventEngineService.triggerCustomGlobalEvent(
          CreateCustomEventEngineService.create(
            CustomEventEditorService.getSelectSceneTreeNodeEventName(),
            None,
          ),
          engineState,
        );
      engineState |> StateEngineService.setState |> ignore;

      StateLogicService.getAndRefreshEngineState();
    };

    dispatchFunc(
      AppStore.UpdateAction(Update([|SceneTree, Inspector, Project|])),
    )
    |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);