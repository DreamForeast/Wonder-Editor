open InspectorComponentType;

let getCameraControllerType = (gameObject, engineState) =>
  engineState
  |> GameObjectComponentEngineService.hasFlyCameraControllerComponent(
       gameObject,
     ) ?
    Some(FlyCameraController) :
    engineState
    |> GameObjectComponentEngineService.hasArcballCameraControllerComponent(
         gameObject,
       ) ?
      Some(ArcballCameraController) : None;

let _updateFlyCameraDirection = (editCamera, engineState) => {
  let flyCameraController =
    engineState
    |> GameObjectComponentEngineService.unsafeGetFlyCameraControllerComponent(
         editCamera,
       );

  FlyCameraEngineService.hasFlyCameraControllerDirection(
    flyCameraController,
    engineState,
  ) ?
    engineState |> StateLogicService.loopBodyWhenStop : engineState;
};

let loopBodyWhenCameraChangeDirection = (editorState, engineState) => {
  let editCamera = editorState |> SceneViewEditorService.unsafeGetEditCamera;

  switch (getCameraControllerType(editCamera, engineState)) {
  | Some(FlyCameraController) =>
    engineState |> _updateFlyCameraDirection(editCamera)
  | _ => engineState
  };
};

let bindCameraControllerEventByType = (gameObject, engineState) =>
  switch (getCameraControllerType(gameObject, engineState)) {
  | Some(FlyCameraController) =>
    engineState
    |> FlyCameraControllerLogicService.bindGameViewActiveCameraFlyCameraControllerEvent(
         gameObject,
       )
  | Some(ArcballCameraController) =>
    engineState
    |> ArcballCameraControllerLogicService.bindGameViewActiveCameraArcballCameraControllerEvent(
         gameObject,
       )
  | None => engineState
  };

let bindGameViewActiveCameraControllerEvent = engineState =>
  switch (
    GameViewEditorService.getActivedBasicCameraView(
      StateEditorService.getState(),
    )
  ) {
  | None => engineState
  | Some(activeBasicCameraView) =>
    let gameObject =
      BasicCameraViewEngineService.getBasicCameraViewGameObject(
        activeBasicCameraView,
        engineState,
      );

    bindCameraControllerEventByType(gameObject, engineState);
  };

let unbindCameraControllerEventByType = (gameObject, engineState) =>
  switch (getCameraControllerType(gameObject, engineState)) {
  | Some(FlyCameraController) =>
    engineState
    |> FlyCameraControllerLogicService.unbindGameViewActiveCameraFlyCameraControllerEvent(
         gameObject,
       )
  | Some(ArcballCameraController) =>
    engineState
    |> ArcballCameraControllerLogicService.unbindGameViewActiveCameraArcballCameraControllerEvent(
         gameObject,
       )
  | None => engineState
  };

let unbindGameViewActiveCameraControllerEvent = engineState =>
  switch (
    GameViewEditorService.getActivedBasicCameraView(
      StateEditorService.getState(),
    )
  ) {
  | None => engineState
  | Some(activeBasicCameraView) =>
    let gameObject =
      BasicCameraViewEngineService.getBasicCameraViewGameObject(
        activeBasicCameraView,
        engineState,
      );
    unbindCameraControllerEventByType(gameObject, engineState);
  };