'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Wonder_jest = require("wonder-bs-jest/lib/js/src/wonder_jest.js");
var TestTool$WonderEditor = require("../../../../../../tool/TestTool.js");
var ReactTestTool$WonderEditor = require("../../../../../../tool/ReactTestTool.js");
var GameObjectTool$WonderEditor = require("../../../../../../tool/GameObjectTool.js");
var StateLogicService$WonderEditor = require("../../../../../../../src/service/stateTuple/logic/StateLogicService.js");
var BuildComponentTool$WonderEditor = require("../../../../../../tool/BuildComponentTool.js");
var MainEditorSceneTool$WonderEditor = require("../../../../../../tool/MainEditorSceneTool.js");

function transformBaseTest(sandbox, describeName, param, param$1) {
  var changeXFunc = param$1[0];
  var getValueFunc = param[1];
  return Wonder_jest.describe(describeName, (function (param) {
                beforeEach((function () {
                        MainEditorSceneTool$WonderEditor.initState(sandbox, undefined, undefined, /* () */0);
                        return MainEditorSceneTool$WonderEditor.createDefaultScene(sandbox, MainEditorSceneTool$WonderEditor.setFirstCubeToBeCurrentSceneTreeNode);
                      }));
                return Wonder_jest.describe("changeX should set current gameObject->transform->value x", (function (param) {
                              Wonder_jest.test("set x value to floatInput", (function (param) {
                                      var currentGameObjectTransform = GameObjectTool$WonderEditor.getCurrentSceneTreeNodeTransform(/* () */0);
                                      Curry._2(changeXFunc, currentGameObjectTransform, -10.1213);
                                      return ReactTestTool$WonderEditor.createSnapshotAndMatch(BuildComponentTool$WonderEditor.buildMainEditorTransformComponent(TestTool$WonderEditor.buildEmptyAppState(/* () */0), currentGameObjectTransform));
                                    }));
                              return Wonder_jest.describe("set engine x value", (function (param) {
                                            Wonder_jest.describe("if value's decimal digits <= 5, can set the whole value to engine", (function (param) {
                                                    Wonder_jest.test("test < 5", (function (param) {
                                                            var currentGameObjectTransform = GameObjectTool$WonderEditor.getCurrentSceneTreeNodeTransform(/* () */0);
                                                            Curry._2(changeXFunc, currentGameObjectTransform, -11.1111);
                                                            var match = StateLogicService$WonderEditor.getEngineStateToGetData(Curry._1(getValueFunc, currentGameObjectTransform));
                                                            return Curry._2(Wonder_jest.Expect[/* Operators */25][/* = */5], Wonder_jest.Expect[/* expect */0](match[0]), -11.1111);
                                                          }));
                                                    Wonder_jest.test("test = 5", (function (param) {
                                                            var currentGameObjectTransform = GameObjectTool$WonderEditor.getCurrentSceneTreeNodeTransform(/* () */0);
                                                            Curry._2(changeXFunc, currentGameObjectTransform, -11.11112);
                                                            var match = StateLogicService$WonderEditor.getEngineStateToGetData(Curry._1(getValueFunc, currentGameObjectTransform));
                                                            return Curry._2(Wonder_jest.Expect[/* Operators */25][/* = */5], Wonder_jest.Expect[/* expect */0](match[0]), -11.11112);
                                                          }));
                                                    return Wonder_jest.test("get the x from engine should == last value", (function (param) {
                                                                  var currentGameObjectTransform = GameObjectTool$WonderEditor.getCurrentSceneTreeNodeTransform(/* () */0);
                                                                  BuildComponentTool$WonderEditor.buildMainEditorTransformComponent(TestTool$WonderEditor.buildEmptyAppState(/* () */0), currentGameObjectTransform);
                                                                  Curry._2(changeXFunc, currentGameObjectTransform, -1.11222);
                                                                  Curry._2(changeXFunc, currentGameObjectTransform, -14.66131);
                                                                  var match = StateLogicService$WonderEditor.getEngineStateToGetData(Curry._1(getValueFunc, currentGameObjectTransform));
                                                                  return Curry._2(Wonder_jest.Expect[/* Operators */25][/* = */5], Wonder_jest.Expect[/* expect */0](match[0]), -14.66131);
                                                                }));
                                                  }));
                                            return Wonder_jest.describe("else", (function (param) {
                                                          return Wonder_jest.test("should set truncated value to engine", (function (param) {
                                                                        var currentGameObjectTransform = GameObjectTool$WonderEditor.getCurrentSceneTreeNodeTransform(/* () */0);
                                                                        Curry._2(changeXFunc, currentGameObjectTransform, -14.6613123);
                                                                        var match = StateLogicService$WonderEditor.getEngineStateToGetData(Curry._1(getValueFunc, currentGameObjectTransform));
                                                                        return Curry._2(Wonder_jest.Expect[/* Operators */25][/* = */5], Wonder_jest.Expect[/* expect */0](match[0]), -14.66131);
                                                                      }));
                                                        }));
                                          }));
                            }));
              }));
}

exports.transformBaseTest = transformBaseTest;
/* Wonder_jest Not a pure module */
