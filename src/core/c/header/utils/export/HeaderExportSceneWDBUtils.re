let generateWDB =
    (
      (rootGameObject, isRootGameObjectRoot, imageUint8ArrayMap),
      generateWDBFunc,
      engineState,
    ) => {
  let isRun = StateEditorService.getIsRun();
  let engineState =
    isRun ?
      engineState :
      engineState
      |> CameraControllerUtils.bindGameViewActiveCameraControllerEvent;

  let engineState =
    engineState
    |> GameObjectEngineService.setGameObjectIsRoot(
         rootGameObject,
         isRootGameObjectRoot,
       );

  let (engineState, _, wdbArrayBuffer) =
    generateWDBFunc(rootGameObject, imageUint8ArrayMap, engineState);

  let engineState =
    isRun ?
      engineState :
      engineState
      |> CameraControllerUtils.unbindGameViewActiveCameraControllerEvent;

  (engineState, wdbArrayBuffer);
};

/* TODO perf: optimize generate scene wdb from cubemapImageUint8ArrayMap */
let generateSceneWDB =
    (isSceneRoot, generateWDBFunc, imageUint8ArrayMap, engineState) =>
  generateWDB(
    (
      SceneEngineService.getSceneGameObject(engineState),
      isSceneRoot,
      imageUint8ArrayMap,
    ),
    generateWDBFunc,
    engineState,
  );