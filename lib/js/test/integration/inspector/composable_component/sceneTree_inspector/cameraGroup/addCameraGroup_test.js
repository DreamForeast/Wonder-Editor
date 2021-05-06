'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Sinon = require("wonder-bs-sinon/lib/js/src/sinon.js");
var Sinon$1 = require("sinon");
var Wonder_jest = require("wonder-bs-jest/lib/js/src/wonder_jest.js");
var GameObjectTool$WonderEditor = require("../../../../../tool/GameObjectTool.js");
var StateLogicService$WonderEditor = require("../../../../../../src/service/stateTuple/logic/StateLogicService.js");
var StateEngineService$WonderEditor = require("../../../../../../src/service/state/engine/state/StateEngineService.js");
var MainEditorSceneTool$WonderEditor = require("../../../../../tool/MainEditorSceneTool.js");
var SceneTreeWidgetService$WonderEditor = require("../../../../../../src/service/record/editor/widget/SceneTreeWidgetService.js");
var BasicCameraViewEngineService$WonderEditor = require("../../../../../../src/service/state/engine/camera/BasicCameraViewEngineService.js");
var CurrentSelectSourceEditorService$WonderEditor = require("../../../../../../src/service/state/editor/CurrentSelectSourceEditorService.js");
var GameObjectComponentEngineService$WonderEditor = require("../../../../../../src/service/state/engine/gameObject/GameObjectComponentEngineService.js");
var MainEditorInspectorAddComponentTool$WonderEditor = require("../../../atom_component/addableComponent/tool/MainEditorInspectorAddComponentTool.js");

Wonder_jest.describe("test add camera group", (function (param) {
        var sandbox = Sinon.getSandboxDefaultVal(/* () */0);
        beforeEach((function () {
                sandbox[0] = Sinon$1.sandbox.create();
                MainEditorSceneTool$WonderEditor.initState(sandbox, undefined, undefined, /* () */0);
                MainEditorSceneTool$WonderEditor.createDefaultScene(sandbox, MainEditorSceneTool$WonderEditor.setFirstCubeToBeCurrentSceneTreeNode);
                var partial_arg = SceneTreeWidgetService$WonderEditor.getWidget(/* () */0);
                return StateLogicService$WonderEditor.getAndSetEditorState((function (param) {
                              return CurrentSelectSourceEditorService$WonderEditor.setCurrentSelectSource(partial_arg, param);
                            }));
              }));
        afterEach((function () {
                return Curry._1(Sinon.restoreSandbox, sandbox[0]);
              }));
        return Wonder_jest.test("shouldn't active basicCameraView", (function (param) {
                      MainEditorInspectorAddComponentTool$WonderEditor.addCameraGroupComponent(undefined, undefined, undefined, /* () */0);
                      var __x = GameObjectTool$WonderEditor.unsafeGetCurrentSceneTreeNode(/* () */0);
                      var __x$1 = GameObjectComponentEngineService$WonderEditor.unsafeGetBasicCameraViewComponent(__x, StateEngineService$WonderEditor.unsafeGetState(/* () */0));
                      return Curry._2(Wonder_jest.Expect[/* Operators */25][/* = */5], Wonder_jest.Expect[/* expect */0](BasicCameraViewEngineService$WonderEditor.isActiveBasicCameraView(__x$1, StateEngineService$WonderEditor.unsafeGetState(/* () */0))), false);
                    }));
      }));

/*  Not a pure module */
