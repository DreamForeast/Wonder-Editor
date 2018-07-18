open Wonder_jest;

open Expect;

open Expect.Operators;

open Sinon;

let _ =
  describe("controller inspector basic material", () => {
    let sandbox = getSandboxDefaultVal();
    beforeEach(() => {
      TestTool.closeContractCheck();
      sandbox := createSandbox();
      MainEditorSceneTool.initStateAndGl(~sandbox, ());
      MainEditorSceneTool.createDefaultScene(
        sandbox,
        () => {
          MainEditorAssetTool.initAssetTree();
          MainEditorSceneTool.setFirstBoxTobeCurrentSceneTreeNode();
        },
      );
      CurrentSelectSourceEditorService.setCurrentSelectSource(
        EditorType.SceneTree,
      )
      |> StateLogicService.getAndSetEditorState;

      EventListenerTool.buildFakeDom()
      |> EventListenerTool.stubGetElementByIdReturnFakeDom;
      ControllerTool.stubRequestAnimationFrame(
        createEmptyStubWithJsObjSandbox(sandbox),
      );
      ControllerTool.run();
    });
    afterEach(() => {
      restoreSandbox(refJsObjToSandbox(sandbox^));
      TestTool.openContractCheck();
    });

    describe("test set value into edit and run engineState", () => {
      let _getGameObjectMaterialSourceSrc = (engineState, gameObject) =>
        engineState
        |> GameObjectComponentEngineService.getBasicMaterialComponent(
             gameObject,
           )
        |. BasicMaterialEngineService.unsafeGetMap(engineState)
        |. BasicSourceTextureEngineService.unsafeGetSource(engineState)
        |. DomHelper.getAttribute("src");

      let _getGameObjectMaterialMap = (engineState, gameObject) =>
        engineState
        |> GameObjectComponentEngineService.getBasicMaterialComponent(
             gameObject,
           )
        |. BasicMaterialEngineService.getMap(engineState);

      test("test drag texture to set gameObject material map", () => {
        let assetTreeDomRecord =
          MainEditorAssetTool.buildTwoLayerAssetTreeRoot();
        let assetState = StateAssetService.getState();

        assetTreeDomRecord
        |> MainEditorAssetNodeTool.OperateTwoLayer.getFirstTextureDomIndex
        |> MainEditorBasicMaterialTool.triggerFileDragStartEvent;

        let dragTextureImageSrc =
          assetState
          |> ImageBase64MapAssetService.getImageBase64Map
          |> WonderCommonlib.SparseMapService.unsafeGet(
               assetState
               |> TextureNodeMapAssetService.getTextureNodeMap
               |> WonderCommonlib.SparseMapService.unsafeGet(
                    assetTreeDomRecord
                    |> MainEditorAssetNodeTool.OperateTwoLayer.getFirstTextureNodeId,
                  )
               |> (({textureIndex}) => textureIndex),
             );

        MainEditorBasicMaterialTool.triggerDragTextureToGameObjectMaterial();

        let currentGameObject =
          SceneEditorService.unsafeGetCurrentSceneTreeNode
          |> StateLogicService.getEditorState;

        let editEngineMaterialSourceSrc =
          _getGameObjectMaterialSourceSrc(
            StateLogicService.getEditEngineState(),
            DiffComponentTool.getEditEngineComponent(
              DiffType.GameObject,
              currentGameObject,
            ),
          );
        let runEngineMaterialSourceSrc =
          _getGameObjectMaterialSourceSrc(
            StateLogicService.getRunEngineState(),
            currentGameObject,
          );

        (editEngineMaterialSourceSrc, runEngineMaterialSourceSrc)
        |> expect == (dragTextureImageSrc, dragTextureImageSrc);
      });

      test("test remove texture", () => {
        let assetTreeDomRecord =
          MainEditorAssetTool.buildTwoLayerAssetTreeRoot();

        assetTreeDomRecord
        |> MainEditorAssetNodeTool.OperateTwoLayer.getFirstTextureDomIndex
        |> MainEditorBasicMaterialTool.triggerFileDragStartEvent;
        MainEditorBasicMaterialTool.triggerDragTextureToGameObjectMaterial();
        MainEditorBasicMaterialTool.triggerTextureRemoveClickEvent();

        let currentGameObject =
          SceneEditorService.unsafeGetCurrentSceneTreeNode
          |> StateLogicService.getEditorState;

        let editEngineMaterialMap =
          _getGameObjectMaterialMap(
            StateLogicService.getEditEngineState(),
            DiffComponentTool.getEditEngineComponent(
              DiffType.GameObject,
              currentGameObject,
            ),
          );
        let runEngineMaterialMap =
          _getGameObjectMaterialMap(
            StateLogicService.getRunEngineState(),
            currentGameObject,
          );

        (editEngineMaterialMap, runEngineMaterialMap)
        |> expect == (None, None);
      });
    });
  });