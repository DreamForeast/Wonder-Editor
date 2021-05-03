open Wonderjs;

open StateDataMainType;

let create = FlyCameraControllerAPI.createFlyCameraController;

let unsafeGetFlyCameraControllerGameObject = FlyCameraControllerAPI.unsafeGetFlyCameraControllerGameObject;

let unsafeGetFlyCameraControllerDirectionArray = FlyCameraControllerAPI.unsafeGetFlyCameraControllerDirectionArray;

let hasFlyCameraControllerDirection = FlyCameraControllerAPI.hasFlyCameraControllerDirection;

let setFlyCameraControllerDirectionArray = FlyCameraControllerAPI.setFlyCameraControllerDirectionArray;

let unsafeGetFlyCameraControllerWheelSpeed = FlyCameraControllerAPI.unsafeGetFlyCameraControllerWheelSpeed;

let setFlyCameraControllerWheelSpeed = FlyCameraControllerAPI.setFlyCameraControllerWheelSpeed;

let unsafeGetFlyCameraControllerMoveSpeed = FlyCameraControllerAPI.unsafeGetFlyCameraControllerMoveSpeed;

let setFlyCameraControllerMoveSpeed = FlyCameraControllerAPI.setFlyCameraControllerMoveSpeed;

let unsafeGetFlyCameraControllerRotateSpeed = FlyCameraControllerAPI.unsafeGetFlyCameraControllerRotateSpeed;

let setFlyCameraControllerRotateSpeed = FlyCameraControllerAPI.setFlyCameraControllerRotateSpeed;

let bindFlyCameraControllerEventForGameView = FlyCameraControllerAPI.bindFlyCameraControllerEvent;

let prepareBindEvent = EventFlyCameraControllerMainService.prepareBindEvent;

let unbindFlyCameraControllerEventForGameView = FlyCameraControllerAPI.unbindFlyCameraControllerEvent;

let isBindFlyCameraControllerEventForGameView = FlyCameraControllerAPI.isBindFlyCameraControllerEvent;

let isTriggerKeydownEventHandler = EventFlyCameraControllerMainService.isTriggerKeydownEventHandler;

let setFlyCameraLocalEulerAngle =
    (transformComponent, targetEulerAngle, engineState) => {
  ...engineState,
  flyCameraControllerRecord:
    engineState.flyCameraControllerRecord
    |> OperateFlyCameraControllerService.setLocalEulerAngle(
         transformComponent,
         targetEulerAngle,
       ),
};
