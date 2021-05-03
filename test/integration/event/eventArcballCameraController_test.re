open Wonder_jest;

open Expect;

open Expect.Operators;

open Sinon;

open EventType;

let _ =
  describe("test arcball cameraController event", () => {
    let sandbox = getSandboxDefaultVal();

    let _prepareMouseEvent = (~sandbox, ()) => {
      MainEditorSceneTool.initStateWithJob(
        ~sandbox,
        ~isInitJob=false,
        ~noWorkerJobRecord=
          NoWorkerJobConfigToolEngine.buildNoWorkerJobConfig(
            ~initPipelines=
              {|
            [
        {
          "name": "default",
          "jobs": [
            {
              "name": "init_event_for_editor"
            },
            {
              "name": "init_camera"
            }
          ]
        }
      ]
            |},
            ~initJobs=
              {j|
    [

        {
              "name": "init_event_for_editor"
        },
            {
              "name": "init_camera"
            }
    ]
            |j},
            (),
          ),
        (),
      );

      MouseEventTool.prepareWithState(
        ~sandbox,
        ~engineState=StateEngineService.unsafeGetState(),
        (),
      );
      MouseEventTool.setPointerLocked(.);
    };

    let _prepareMouseEventForTestKeyboardEvent =
        (~sandbox, ~bindEventFunc, ()) => {
      _prepareMouseEvent(~sandbox, ());
      MouseEventTool.prepareForPointerLock(~sandbox, ());

      let engineState = StateEngineService.unsafeGetState();
      let (
        engineState,
        gameObject,
        transform,
        (cameraController, basicCameraView, perspectiveCameraProjection),
      ) =
        ArcballCameraControllerToolEngine.createGameObject(engineState);

      let (posX, posY, posZ) as pos = (1., 2., 3.);
      let engineState =
        engineState |> TransformEngineService.setLocalPosition(pos, transform);
      let target = pos;
      let moveSpeedX = 0.1;
      let moveSpeedY = 0.2;
      let engineState =
        engineState
        |> ArcballCameraEngineService.setArcballCameraControllerTarget(
             cameraController,
             target,
           )
        |> ArcballCameraEngineService.setArcballCameraControllerMoveSpeedX(
             cameraController,
             moveSpeedX,
           )
        |> ArcballCameraEngineService.setArcballCameraControllerMoveSpeedY(
             cameraController,
             moveSpeedY,
           );

      PrepareRenderViewJobTool.setViewRect(~width=100, ~height=50, ());
      let engineState = engineState |> MainUtils._handleEngineState;

      let engineState = bindEventFunc(cameraController, engineState);

      engineState |> StateEngineService.setState |> ignore;

      (cameraController, (moveSpeedX, moveSpeedY), pos);
    };

    let _prepareForPointerLock = sandbox =>
      MouseEventTool.prepareForPointerLock(~sandbox, ());

    let _testPointDragStartEvent =
        (sandbox, (pageX, pageY, eventButton), (judgeFunc, bindEventFunc)) => {
      _prepareMouseEvent(~sandbox, ());

      let requestPointerLockStub = _prepareForPointerLock(sandbox);
      let engineState = StateEngineService.unsafeGetState();
      let (engineState, _, _, (cameraController, _, _)) =
        ArcballCameraControllerToolEngine.createGameObject(engineState);

      let engineState = bindEventFunc(cameraController, engineState);

      engineState |> StateEngineService.setState |> ignore;

      PrepareRenderViewJobTool.setViewRect(~width=100, ~height=50, ());
      StateLogicService.getAndSetEngineState(MainUtils._handleEngineState);

      EventTool.triggerDomEvent(
        "mousedown",
        EventTool.getBody(),
        MouseEventTool.buildMouseDomEvent(
          ~pageX,
          ~pageY,
          ~which=eventButton,
          (),
        ),
      );
      EventTool.restore();

      judgeFunc(requestPointerLockStub);
    };

    let _execKeydownEvent =
        (
          ~pageX,
          ~pageY,
          ~keyCode,
          ~ctrlKey=false,
          ~preventDefaultFunc=createEmptyStubWithJsObjSandbox(sandbox),
          (),
        ) => {
      EventTool.triggerDomEvent(
        "mousedown",
        EventTool.getBody(),
        MouseEventTool.buildMouseDomEvent(~pageX, ~pageY, ()),
      );
      EventTool.triggerDomEvent(
        "keydown",
        EventTool.getKeyboardEventBindedDom(),
        KeyboardEventTool.buildKeyboardDomEvent(
          ~keyCode,
          ~ctrlKey,
          ~preventDefaultFunc,
          (),
        ),
      );
      EventTool.restore();
    };

    let _testKeydownEvent =
        (sandbox, (pageX, pageY), prepareMouseEventFunc, directionArray) => {
      let (cameraController, (moveSpeedX, moveSpeedY), (posX, posY, posZ)) =
        prepareMouseEventFunc(~sandbox, ());

      _execKeydownEvent(~pageX, ~pageY, ~ctrlKey=false, ~keyCode=65, ());

      let engineState = StateEngineService.unsafeGetState();
      engineState
      |> ArcballCameraEngineService.unsafeGetArcballCameraControllerDirectionArray(
           cameraController,
         )
      |> expect == directionArray;
    };

    beforeEach(() => sandbox := createSandbox());
    afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));

    describe("test bind for game view", () => {
      describe("test bind point drag start event", () => {
        let _test = (sandbox, (pageX, pageY, eventButton), judgeFunc) =>
          _testPointDragStartEvent(
            sandbox,
            (pageX, pageY, eventButton),
            (
              judgeFunc,
              ArcballCameraEngineService.bindArcballCameraControllerEventForGameView,
            ),
          );

        test(
          "if eventTarget is scene view, not request canvas pointerLock", () =>
          _test(sandbox, (10, 20, 3), requestPointerLockStub =>
            requestPointerLockStub |> expect |> not_ |> toCalled
          )
        );

        describe("if eventTarget is game view", () => {
          test("if mouse button isn't right button, still trigger", () =>
            _test(sandbox, (60, 20, 1), requestPointerLockStub =>
              requestPointerLockStub |> expect |> toCalledOnce
            )
          );
          test("if eventTarget is game view, request canvas pointerLock", () =>
            _test(sandbox, (60, 20, 3), requestPointerLockStub =>
              requestPointerLockStub |> expect |> toCalledOnce
            )
          );
        });
      });

      describe("test bind keydown event", () => {
        let _prepareMouseEvent = (~sandbox, ()) =>
          _prepareMouseEventForTestKeyboardEvent(
            ~sandbox,
            ~bindEventFunc=ArcballCameraEngineService.bindArcballCameraControllerEventForGameView,
            (),
          );

        describe("test set target", () => {
          test("if eventTarget is scene view, not move", () =>
            _testKeydownEvent(sandbox, (10, 20), _prepareMouseEvent, [||])
          );
          test("if eventTarget is game view, move", () =>
            _testKeydownEvent(
              sandbox,
              (60, 20),
              _prepareMouseEvent,
              [|Wonderjs.ArcballCameraControllerType.Left|],
            )
          );
        });
      });
    });

    describe("test bind for inspector", () =>
      describe("test bind point drag start event", () => {
        let _testPointDragStartEvent =
            (
              sandbox,
              (pageX, pageY, eventButton),
              (judgeFunc, bindEventFunc),
            ) => {
          InspectorCanvasEventTool.prepareMouseEvent(~sandbox, ());

          let requestPointerLockStub =
            MouseEventTool.prepareForPointerLock(
              ~sandbox,
              ~unsafeGetStateFunc=StateInspectorEngineService.unsafeGetState,
              (),
            );
          let inspectorEngineState =
            StateInspectorEngineService.unsafeGetState();
          let (inspectorEngineState, _, _, (cameraController, _, _)) =
            ArcballCameraControllerToolEngine.createGameObject(
              inspectorEngineState,
            );

          let inspectorEngineState =
            bindEventFunc(cameraController, inspectorEngineState);

          inspectorEngineState
          |> StateInspectorEngineService.setState
          |> ignore;

          StateLogicService.getAndSetEngineState(
            MainUtils._handleEngineState,
          );

          EventTool.triggerDomEvent(
            "mousedown",
            EventTool.getBody(),
            MouseEventTool.buildMouseDomEvent(
              ~pageX,
              ~pageY,
              ~which=eventButton,
              (),
            ),
          );
          EventTool.restore();

          judgeFunc(requestPointerLockStub);
        };

        let _test = (sandbox, (pageX, pageY, eventButton), judgeFunc) =>
          _testPointDragStartEvent(
            sandbox,
            (pageX, pageY, eventButton),
            (
              judgeFunc,
              ArcballCameraControllerLogicService.bindArcballCameraControllerEventForInspector,
            ),
          );

        test("if mouse button isn't right button, still trigger", () =>
          _test(sandbox, (10, 20, 1), requestPointerLockStub =>
            requestPointerLockStub |> expect |> toCalledOnce
          )
        );
      })
    );
  });