let _removeComponent = ((uiState, dispatchFunc), gameObject, type_) =>
  ComponentBox.Method.removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    type_,
  );

let removeDirectionLightComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.Light,
  );

let removeCameraGroupComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.CameraGroup,
  );

let removeGeometryComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.Geometry,
  );

let removeRenderGroupComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.RenderGroup,
  );

let removeFlyCameraControllerComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.FlyCameraController,
  );

let removeArcballCameraControllerComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.ArcballCameraController,
  );

let removeScriptComponent =
    (
      ~uiState=TestTool.buildEmptyAppState(),
      ~dispatchFunc=TestTool.getDispatch(),
      ~gameObject=GameObjectTool.unsafeGetCurrentSceneTreeNode(),
      (),
    ) =>
  _removeComponent(
    (uiState, dispatchFunc),
    gameObject,
    InspectorComponentType.Script,
  );