

import * as Curry from "../../../../../../../../../node_modules/bs-platform/lib/es6/curry.js";
import * as Pervasives from "../../../../../../../../../node_modules/bs-platform/lib/es6/pervasives.js";
import * as Log$WonderLog from "../../../../../../../../../node_modules/wonder-log/lib/es6_global/src/Log.js";
import * as ArrayService$WonderEditor from "../../../../../../service/atom/ArrayService.js";
import * as ArrayService$WonderCommonlib from "../../../../../../../../../node_modules/wonder-commonlib/lib/es6_global/src/ArrayService.js";
import * as GameObjectComponentEngineService$WonderEditor from "../../../../../../service/state/engine/GameObjectComponentEngineService.js";

function _isAdded(component) {
  return component !== -1;
}

function _getNotAddedComponent() {
  return -1;
}

function _getComponent(gameObject, hasComponent, getComponent, engineState) {
  var match = Curry._2(hasComponent, gameObject, engineState);
  if (match) {
    return Curry._2(getComponent, gameObject, engineState);
  } else {
    return -1;
  }
}

function _operateSpecificComponent(gameObject, componentName, engineState) {
  switch (componentName) {
    case "basicCameraView" : 
        return _getComponent(gameObject, GameObjectComponentEngineService$WonderEditor.hasBasicCameraViewComponent, GameObjectComponentEngineService$WonderEditor.getBasicCameraViewComponent, engineState);
    case "basicMaterial" : 
        return _getComponent(gameObject, GameObjectComponentEngineService$WonderEditor.hasBasicMaterialComponent, GameObjectComponentEngineService$WonderEditor.getBasicMaterialComponent, engineState);
    case "boxGeometry" : 
        return _getComponent(gameObject, GameObjectComponentEngineService$WonderEditor.hasBoxGeometryComponent, GameObjectComponentEngineService$WonderEditor.getGeometryComponent, engineState);
    case "perspectiveCameraProjection" : 
        return _getComponent(gameObject, GameObjectComponentEngineService$WonderEditor.hasPerspectiveCameraProjectionComponent, GameObjectComponentEngineService$WonderEditor.getPerspectiveCameraProjectionComponent, engineState);
    case "sourceInstance" : 
        return _getComponent(gameObject, GameObjectComponentEngineService$WonderEditor.hasSourceInstanceComponent, GameObjectComponentEngineService$WonderEditor.getSourceInstanceComponent, engineState);
    case "transform" : 
        return _getComponent(gameObject, GameObjectComponentEngineService$WonderEditor.hasTransformComponent, GameObjectComponentEngineService$WonderEditor.getTransformComponent, engineState);
    default:
      return Log$WonderLog.fatal(Log$WonderLog.buildErrorMessage("_getGameObjectSpecificComponent", "specific component:" + (String(componentName) + " is error"), "", "", "gameObject:" + (String(gameObject) + (", component:" + (String(componentName) + "")))));
  }
}

function _isSpecificComponentExist(includeComponent, excludeComponent, gameObject, engineState) {
  var len = includeComponent.filter((function (item) {
          return _operateSpecificComponent(gameObject, item, engineState) !== -1;
        })).length;
  if (len === includeComponent.length) {
    return excludeComponent.filter((function (item) {
                  return _operateSpecificComponent(gameObject, item, engineState) !== -1;
                })).length === 0;
  } else {
    return false;
  }
}

function buildCurrentSceneTreeNodeShowComponentList(gameObject, allShowComponentConfig, engineState) {
  var gameObjectType = ArrayService$WonderEditor.getFirst(allShowComponentConfig.filter((function (gameObjectType) {
              return _isSpecificComponentExist(gameObjectType[/* include_component */1], gameObjectType[/* exclude_component */2], gameObject, engineState);
            })));
  return ArrayService$WonderCommonlib.reduceOneParam((function (param, item) {
                var addableComponentList = param[1];
                var addedComponentList = param[0];
                var component = _operateSpecificComponent(gameObject, item[/* type_ */0], engineState);
                var match = component !== -1;
                if (match) {
                  return /* tuple */[
                          Pervasives.$at(addedComponentList, /* :: */[
                                /* tuple */[
                                  item[/* type_ */0],
                                  component
                                ],
                                /* [] */0
                              ]),
                          addableComponentList
                        ];
                } else {
                  return /* tuple */[
                          addedComponentList,
                          Pervasives.$at(addableComponentList, /* :: */[
                                item[/* type_ */0],
                                /* [] */0
                              ])
                        ];
                }
              }), /* tuple */[
              /* [] */0,
              /* [] */0
            ], gameObjectType[/* all_component */3]);
}

export {
  _isAdded ,
  _getNotAddedComponent ,
  _getComponent ,
  _operateSpecificComponent ,
  _isSpecificComponentExist ,
  buildCurrentSceneTreeNodeShowComponentList ,
  
}
/* Log-WonderLog Not a pure module */
