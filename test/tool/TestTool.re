let getDispatch = () => Reductive.Store.dispatch(IndexStore.store);

let buildEmptyAppState = () => AppStore.state;

let rebuildAssetState = () =>
  StateAssetService.setState(CreateAssetStateAssetService.create());

let _buildSceneTreeAppState = sceneGraphData => {
  let state = buildEmptyAppState();
  {
    ...state,
    sceneTreeState: {
      ...state.sceneTreeState,
      sceneGraphData: Some(sceneGraphData),
    },
  };
};

let buildAppStateSceneGraphFromEngine = () =>
  (
    stateTuple =>
      stateTuple
      |> SceneTreeUtils.getSceneGraphDataFromEngine
      |> _buildSceneTreeAppState
  )
  |> StateLogicService.getStateToGetData;

let createScene = () => {
  let engineForEditState = StateLogicService.getEditEngineState();
  let (engineForEditState, editEngineStateScene) =
    GameObjectEngineService.create(engineForEditState);

  engineForEditState
  |> GameObjectEngineService.setGameObjectName("scene", editEngineStateScene)
  |> StateLogicService.setEditEngineState;

  let engineForRunState = StateLogicService.getRunEngineState();
  let (engineForRunState, runEngineStateScene) =
    GameObjectEngineService.create(engineForRunState);

  engineForRunState
  |> GameObjectEngineService.setGameObjectName("scene", runEngineStateScene)
  |> StateLogicService.setRunEngineState;

  let editorState = StateEditorService.getState();
  editorState
  |> SceneEditorService.setScene(runEngineStateScene)
  |> StateEditorService.setState
  |> ignore;
};

let initEditorAndEngineStateAndInitScene = (~sandbox, ~buffer) => {
  TestToolEngine.createAndSetEngineState(~sandbox, ~buffer, ());
  createScene();
};

let openContractCheck = () =>
  CreateEditorStateDataEditorService.editorStateData.isDebug = true;

let closeContractCheck = () =>
  CreateEditorStateDataEditorService.editorStateData.isDebug = false;