

import * as Curry from "../../../../../../../node_modules/bs-platform/lib/es6/curry.js";
import * as Caml_option from "../../../../../../../node_modules/bs-platform/lib/es6/caml_option.js";
import * as BufferUtils$WonderEditor from "../../../../core/composable_component/header/utils/BufferUtils.js";

function getBase64ForWhiteImage(param) {
  return "data:image/png;base64,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";
}

function getUint8Array(param, buildDefaultMaterialSnapshotUint8ArrayFunc) {
  var uint8Array = param[/* uint8Array */1];
  if (uint8Array !== undefined) {
    return Caml_option.valFromOption(uint8Array);
  } else {
    var base64 = param[/* base64 */0];
    if (base64 !== undefined) {
      return BufferUtils$WonderEditor.convertBase64ToUint8Array(base64);
    } else {
      return Curry._1(buildDefaultMaterialSnapshotUint8ArrayFunc, /* () */0);
    }
  }
}

export {
  getBase64ForWhiteImage ,
  getUint8Array ,
  
}
/* BufferUtils-WonderEditor Not a pure module */
