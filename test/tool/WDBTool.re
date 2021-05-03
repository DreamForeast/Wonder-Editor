let convertGLBToWDB = glbName => {
  let glbArrayBuffer = GLBTool.getGLBArrayBuffer(glbName);

  LoadTool.buildFakeTextDecoder(LoadTool.convertUint8ArrayToBuffer);
  LoadTool.buildFakeTextEncoder(.);

  glbArrayBuffer |> Wonderjs.ConverterAPI.convertGLBToWDB;
};

let _createPointLight = (editorState, engineState) => {
  let (editorState, (engineState, obj)) =
    GameObjectLogicService.createGameObject((editorState, engineState));
  let (engineState, pointLight) =
    PointLightEngineService.create(engineState);

  let engineState =
    engineState
    |> GameObjectEngineService.setGameObjectName("Point Light", obj);

  let (editorState, engineState) =
    (editorState, engineState)
    |> GameObjectLogicService.addPointLight(obj, pointLight);

  (editorState, engineState, obj);
};

let _createStateTuple = () => {
  let sandbox = Sinon.createSandbox();

  MainEditorSceneTool.initStateWithJob(
    ~sandbox=ref(sandbox),
    ~isBuildFakeDom=false,
    ~noWorkerJobRecord=
      NoWorkerJobConfigToolEngine.buildNoWorkerEmptyJobConfig(),
    (),
  );

  let engineState = StateEngineService.unsafeGetState();

  let engineState =
    InitEditorJob.initEditorJob([||], StateEngineService.unsafeGetState());

  let editorState = StateEditorService.getState();

  let engineState =
    engineState
    |> FakeGlToolEngine.setFakeGl(
         FakeGlToolEngine.buildFakeGl(~sandbox=ref(sandbox), ()),
       );

  (editorState, engineState);
};

let _buildFakeCanvas = (sandbox, base64, callIndex) => {
  open Sinon;

  let toDataURLStub = createEmptyStubWithJsObjSandbox(sandbox);
  toDataURLStub |> returns(base64);

  let canvasDom = {
    "width": 0,
    "height": 0,
    "getContext": () => {
      "drawImage": createEmptyStubWithJsObjSandbox(sandbox),
      "clearRect": createEmptyStubWithJsObjSandbox(sandbox),
    },
    "toDataURL": toDataURLStub,
  };

  canvasDom;
};

let _prepareFakeCanvas = sandbox => {
  open Sinon;

  let base64_1 = "data:image/png;base64,aaaacccccccccccccccccccccccaaacccccccccccccccccccccccaaacccccccccccccccccccccccaacccccccccccccccccccccccaaaacccccccccccccccccccccccaaacccccccccccccccccccccccaaacccccccccccccccccccccccaaccccccccccccccccccccccc";
  let base64_2 = "data:image/jpeg;base64,bbb";
  let base64_3 = "data:image/png;base64,aaa";
  let base64_4 = "data:image/png;base64,ccc";
  let base64_5 = "data:image/png;base64,ddd";
  let base64_6 = "data:image/jpeg;base64,ccc";
  let base64_7 = "data:image/png;base64,azc";

  let canvas1 = _buildFakeCanvas(sandbox, base64_1, 0);
  let canvas2 = _buildFakeCanvas(sandbox, base64_2, 1);
  let canvas3 = _buildFakeCanvas(sandbox, base64_3, 2);
  let canvas4 = _buildFakeCanvas(sandbox, base64_4, 3);
  let canvas5 = _buildFakeCanvas(sandbox, base64_5, 4);
  let canvas6 = _buildFakeCanvas(sandbox, base64_6, 5);
  let canvas7 = _buildFakeCanvas(sandbox, base64_7, 6);

  let createElementStub =
    createMethodStub(
      refJsObjToSandbox(sandbox^),
      DomHelper.document |> Obj.magic,
      "createElement",
    );

  createElementStub
  |> withOneArg("canvas")
  |> onCall(0)
  |> returns(canvas1)
  |> onCall(1)
  |> returns(canvas2)
  |> onCall(2)
  |> returns(canvas3)
  |> onCall(3)
  |> returns(canvas4)
  |> onCall(4)
  |> returns(canvas5)
  |> onCall(5)
  |> returns(canvas6)
  |> onCall(6)
  |> returns(canvas7)
  |> ignore;

  (base64_1, base64_2, base64_3, base64_4, base64_5, base64_6, base64_7);
};

let generateWDB = buildWDBGameObjectFunc => {
  open Sinon;

  let (editorState, engineState) = _createStateTuple();

  let sandbox = ref(createSandbox());
  let _ = _prepareFakeCanvas(sandbox);

  let (rootGameObject, (editorState, engineState)) =
    buildWDBGameObjectFunc(editorState, engineState);

  LoadTool.buildFakeTextEncoder(.);

  let (engineState, wdbArrayBuffer) =
    HeaderExportSceneWDBUtils.generateWDB(
      (
        rootGameObject,
        true,
        Js.Nullable.return(
          Uint8ArrayAssetEditorService.buildImageUint8ArrayMap(editorState),
        ),
      ),
      GenerateSceneGraphEngineService.generateWDB,
      engineState,
    );

  restoreSandbox(refJsObjToSandbox(sandbox^));

  wdbArrayBuffer;
};

/* let generateSceneWDB =
     (
       ~isSceneRoot=false,
       ~generateWDBFunc=GenerateSceneGraphEngineService.generateWDB,
       ~imageUint8ArrayMap=Js.Nullable.return(
                             Uint8ArrayAssetEditorService.buildImageUint8ArrayMap(
                               StateEditorService.getState(),
                             ),
                           ),
       ~engineState=StateEngineService.unsafeGetState(),
       (),
     ) =>
   HeaderExportSceneWDBUtils.generateSceneWDB(
     isSceneRoot,
     generateWDBFunc,
     imageUint8ArrayMap,
     engineState,
   ); */

let buildSource = (~width=1, ~height=2, ~name="image.png", ()) =>
  {"width": width, "height": height, "name": name} |> Obj.magic;

let generateDirectionPointLightsAndCubeWDB = () =>
  generateWDB((editorState, engineState) => {
    let (engineState, geometry) =
      GeometryEngineService.createCubeGeometry(engineState);
    let (engineState, lightMaterial) =
      LightMaterialEngineService.create(engineState);

    let (editorState, engineState, cube1) =
      PrimitiveLogicService.createCube(
        (geometry, lightMaterial),
        editorState,
        engineState,
      );

    let (editorState, engineState, directionLight) =
      PrimitiveLogicService.createDirectionLight(editorState, engineState);

    let (editorState, engineState, pointLight) =
      _createPointLight(editorState, engineState);

    let (engineState, rootGameObject) =
      GameObjectEngineService.create(engineState);

    let engineState =
      engineState
      |> HierarchyGameObjectEngineService.addChild(rootGameObject, cube1)
      |> HierarchyGameObjectEngineService.addChild(
           rootGameObject,
           directionLight,
         )
      |> HierarchyGameObjectEngineService.addChild(rootGameObject, pointLight);

    (rootGameObject, (editorState, engineState));
  });

let _generateWDBWithCameraType =
    (createCameraControllerFunc, bindEventFunc, addCameraControllerFunc) =>
  generateWDB((editorState, engineState) => {
    let engineState =
      ManageIMGUIEngineService.setIMGUIFunc(
        Obj.magic(Js.Nullable.null),
        Obj.magic((. _, apiJsObj, engineState) => {
          let label = apiJsObj##label;
          let engineState =
            label(. (100., 30., 300., 200.), "imgui", 0, engineState);

          engineState;
        }),
        engineState,
      );

    let (engineState, geometry) =
      GeometryEngineService.createCubeGeometry(engineState);
    let (engineState, lightMaterial) =
      LightMaterialEngineService.create(engineState);

    let (editorState, engineState, cube1) =
      PrimitiveLogicService.createCube(
        (geometry, lightMaterial),
        editorState,
        engineState,
      );

    let (editorState, engineState, cube2) =
      PrimitiveLogicService.createCube(
        (geometry, lightMaterial),
        editorState,
        engineState,
      );

    let (editorState, engineState, camera) =
      CameraLogicService.createCamera(editorState, engineState);
    let (editorState, engineState, camera2) =
      CameraLogicService.createCamera(editorState, engineState);
    let (engineState, cameraController) =
      createCameraControllerFunc(engineState);

    let basicCameraViewComponent =
      GameObjectComponentEngineService.unsafeGetBasicCameraViewComponent(
        camera,
        engineState,
      );

    let engineState =
      engineState
      |> TransformEngineService.setLocalPosition(
           (20., 0., 100.),
           GameObjectComponentEngineService.unsafeGetTransformComponent(
             camera2,
             engineState,
           ),
         )
      |> bindEventFunc(cameraController)
      |> BasicCameraViewEngineService.activeBasicCameraView(
           basicCameraViewComponent,
         );

    let (editorState, engineState) =
      (editorState, engineState)
      |> addCameraControllerFunc(camera, cameraController);

    let (editorState, engineState, directionLight) =
      PrimitiveLogicService.createDirectionLight(editorState, engineState);

    let (engineState, rootGameObject) =
      GameObjectEngineService.create(engineState);

    let engineState =
      engineState
      |> HierarchyGameObjectEngineService.addChild(rootGameObject, cube1)
      |> HierarchyGameObjectEngineService.addChild(cube1, camera2)
      |> HierarchyGameObjectEngineService.addChild(rootGameObject, camera)
      |> HierarchyGameObjectEngineService.addChild(
           rootGameObject,
           directionLight,
         );

    (rootGameObject, (editorState, engineState));
  });

let generateSceneWDBWithArcballCameraController = () =>
  _generateWDBWithCameraType(
    ArcballCameraEngineService.create,
    ArcballCameraControllerLogicService.bindArcballCameraControllerEventForSceneView,
    GameObjectLogicService.addArcballCameraController,
  );

let generateSceneWDBWithFlyCameraController = () =>
  _generateWDBWithCameraType(
    FlyCameraEngineService.create,
    FlyCameraControllerLogicService.bindFlyCameraControllerEventForSceneView,
    GameObjectLogicService.addFlyCameraController,
  );

module ScriptEventFunction = {
  let getScriptGameObjectName = () => "ScriptGameObject";

  let generateScriptEventFunctionWDB =
      (scriptEventFunctionDataName, scriptEventFunctionData) =>
    generateWDB((editorState, engineState) => {
      let (engineState, rootGameObject) =
        GameObjectEngineService.create(engineState);

      let (engineState, gameObject1) =
        GameObjectEngineService.create(engineState);

      let engineState =
        GameObjectEngineService.setGameObjectName(
          getScriptGameObjectName(),
          gameObject1,
          engineState,
        );

      let (engineState, script) = ScriptEngineService.create(engineState);
      let engineState =
        engineState
        |> GameObjectComponentEngineService.addScriptComponent(
             gameObject1,
             script,
           );

      let engineState =
        ScriptEngineService.addScriptEventFunctionData(
          script,
          scriptEventFunctionDataName,
          scriptEventFunctionData,
          engineState,
        );

      let engineState =
        engineState
        |> HierarchyGameObjectEngineService.addChild(
             rootGameObject,
             gameObject1,
           );

      (rootGameObject, (editorState, engineState));
    });

  let getScriptGameObject = engineState =>
    LoadWDBTool.findGameObjectByName(getScriptGameObjectName(), engineState);

  let getScriptGameObjectByWDBGameObject = (rootGameObject, engineState) =>
    GameObjectToolEngine.findGameObjectByName(
      getScriptGameObjectName(),
      rootGameObject,
      engineState,
    )
    |> ArrayService.unsafeGetFirst;
};

module ScriptAttribute = {
  let getScriptGameObjectName = () => "ScriptGameObject";

  let createRootGameObjectForGenerateScriptAttributeWDB =
      (scriptAttributeName, scriptAttribute, engineState) => {
    let (engineState, rootGameObject) =
      GameObjectEngineService.create(engineState);

    let (engineState, gameObject1) =
      GameObjectEngineService.create(engineState);

    let engineState =
      GameObjectEngineService.setGameObjectName(
        getScriptGameObjectName(),
        gameObject1,
        engineState,
      );

    let (engineState, script) = ScriptEngineService.create(engineState);
    let engineState =
      engineState
      |> GameObjectComponentEngineService.addScriptComponent(
           gameObject1,
           script,
         );

    let engineState =
      ScriptEngineService.addScriptAttribute(
        script,
        scriptAttributeName,
        scriptAttribute,
        engineState,
      );

    let engineState =
      engineState
      |> HierarchyGameObjectEngineService.addChild(
           rootGameObject,
           gameObject1,
         );

    (rootGameObject, engineState);
  };

  let createRootGameObjectForGenerateScriptAttributeWDB2 =
      (scriptAttributeName, scriptAttribute, engineState) => {
    let (engineState, rootGameObject) =
      GameObjectEngineService.create(engineState);

    let (engineState, gameObject1) =
      GameObjectEngineService.create(engineState);

    let (engineState, gameObject2) =
      GameObjectEngineService.create(engineState);

    let engineState =
      GameObjectEngineService.setGameObjectName(
        getScriptGameObjectName(),
        gameObject1,
        engineState,
      );

    let (engineState, script) = ScriptEngineService.create(engineState);
    let engineState =
      engineState
      |> GameObjectComponentEngineService.addScriptComponent(
           gameObject1,
           script,
         );

    let engineState =
      ScriptEngineService.addScriptAttribute(
        script,
        scriptAttributeName,
        scriptAttribute,
        engineState,
      );

    let engineState =
      engineState
      |> HierarchyGameObjectEngineService.addChild(
           rootGameObject,
           gameObject1,
         );

    let engineState =
      engineState
      |> HierarchyGameObjectEngineService.addChild(
           rootGameObject,
           gameObject2,
         );

    (rootGameObject, engineState);
  };

  let generateScriptAttributeWDB = (scriptAttributeName, scriptAttribute) =>
    generateWDB((editorState, engineState) => {
      let (rootGameObject, engineState) =
        createRootGameObjectForGenerateScriptAttributeWDB(
          scriptAttributeName,
          scriptAttribute,
          engineState,
        );

      (rootGameObject, (editorState, engineState));
    });

  let getScriptGameObject = engineState =>
    LoadWDBTool.findGameObjectByName(getScriptGameObjectName(), engineState);
};