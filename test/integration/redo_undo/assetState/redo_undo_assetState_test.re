open Wonder_jest;

open Expect;

open Expect.Operators;

open Sinon;

let _ =
  describe("redo_undo: assetState", () => {
    let sandbox = getSandboxDefaultVal();
    beforeEach(() => {
      sandbox := createSandbox();

      TestTool.closeContractCheck();
      MainEditorSceneTool.initStateAndGl(~sandbox, ());
      MainEditorSceneTool.createDefaultScene(sandbox, () => ());
      StateHistoryToolEditor.clearAllState();

      SceneTreeNodeDomTool.OperateTwoLayer.getFirstCubeDomIndex()
      |> SceneTreeTool.clearCurrentGameObjectAndSetTreeSpecificGameObject;
    });
    afterEach(() => {
      TestTool.openContractCheck();
      restoreSandbox(refJsObjToSandbox(sandbox^));
      StateAssetService.getState()
      |> CurrentNodeDataAssetService.clearCurrentNodeData
      |> CurrentNodeParentIdAssetService.clearCurrentNodeParentId
      |> StateAssetService.setState
      |> ignore;
    });
    describe("prepare first step: set currentSceneTreeNode", () => {
      let _simulateAddGameObjectTwice = () => {
        let headerComponent =
          BuildComponentTool.buildHeader(
            TestTool.buildAppStateSceneGraphFromEngine(),
          );
        BaseEventTool.triggerComponentEvent(
          headerComponent,
          OperateGameObjectEventTool.triggerClickAddBox,
        );
        BaseEventTool.triggerComponentEvent(
          headerComponent,
          OperateGameObjectEventTool.triggerClickAddBox,
        );
      };
      describe("test assetState change", () => {
        describe("test operate gameObject should not change assetState", () => {
          test("test add gameObject, the assetState not change", () => {
            let assetState1 = StateAssetService.getState();

            _simulateAddGameObjectTwice();

            let assetState2 = StateAssetService.getState();

            assetState1 |> expect == assetState2;
          });
          test("undo one step,should not change assetState", () => {
            let assetState1 = StateAssetService.getState();

            _simulateAddGameObjectTwice();
            StateHistoryToolEditor.undo();

            let assetState2 = StateAssetService.getState();

            assetState1 |> expect == assetState2;
          });
          test("redo one step,should not change assetState", () => {
            let assetState1 = StateAssetService.getState();

            _simulateAddGameObjectTwice();
            StateHistoryToolEditor.undo();
            StateHistoryToolEditor.redo();

            let assetState2 = StateAssetService.getState();

            assetState1 |> expect == assetState2;
          });
        });
        describe("test operate asset, should change assetState", () => {
          beforeEach(() => {
            MainEditorAssetTool.buildTwoLayerAssetTreeRoot() |> ignore;
            MainEditorAssetTool.buildFakeFileReader();
            MainEditorAssetTool.buildFakeImage();
          });
          testPromise("test load file should change assetState", () => {
            let assetState1 = StateAssetService.getState();

            MainEditorAssetHeader.Method._fileLoad(
              TestTool.getDispatch(),
              BaseEventTool.buildFileEvent(),
            )
            |> Js.Promise.then_(_ => {
                 let assetState2 = StateAssetService.getState();

                 assetState1 |> expect != assetState2 |> Js.Promise.resolve;
               });
          });
          testPromise("test undo operate should not change assetState", () =>
            MainEditorAssetHeader.Method._fileLoad(
              TestTool.getDispatch(),
              BaseEventTool.buildFileEvent(),
            )
            |> Js.Promise.then_(_ => {
                 let assetState1 = StateAssetService.getState();

                 StateHistoryToolEditor.undo();

                 let assetState2 = StateAssetService.getState();

                 assetState1 |> expect == assetState2 |> Js.Promise.resolve;
               })
          );
        });
      });
    });
  });