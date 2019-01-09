open Wonder_jest;

open Expect;

open Expect.Operators;

open Sinon;

let _ =
  describe("redo_undo:leftHeader add gameObject", () => {
    let sandbox = getSandboxDefaultVal();

    let _simulateTwiceAddGameObject = () => {
      MainEditorLeftHeaderTool.addCube();
      MainEditorLeftHeaderTool.addEmptyGameObject();
    };
    let _beforeEach = () => {
      MainEditorSceneTool.initState(~sandbox, ());
      MainEditorSceneTool.createDefaultScene(
        sandbox,
        MainEditorSceneTool.setFirstBoxToBeCurrentSceneTreeNode,
      );
    };

    beforeEach(() => sandbox := createSandbox());
    afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));

    RedoUndoTool.testRedoUndoTwoStep(
      sandbox,
      "prepare first step: set currentSceneTreeNode",
      (_simulateTwiceAddGameObject, _beforeEach, () => ()),
      BuildComponentForCurryTool.buildSceneTree,
    );
  });