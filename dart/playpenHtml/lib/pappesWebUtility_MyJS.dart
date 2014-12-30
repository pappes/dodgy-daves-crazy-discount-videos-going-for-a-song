// Copyright (c) 2015, pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// Collection of utility classes and functions to make programs simpler.
part of pappesWebUtility;



/// Utility functions to abstract working with the browsers JavaScript engine.
///
/// ## purpose 
/// 
/// * Extend built-in dart JavaScript functionality.
///   


class MyJS {
  //implimented as a singleton as all instances would behave the same anyway
  static final _singleton = new MyJS._initialise();

  /// Converts [val] from base64 to text.
  ///
  /// if [val] is not valid base64 then returns null
  /// if library dart:js is not working throws StateError
  /// 
  ///    print(MyJS.base64Decode('SGVsbG8gV29ybGQ='));
  ///    
  static String base64Decode(String val) {
    bool jsHasAtob;
    try {
      jsHasAtob = js.context.hasProperty('atob');
    } catch (e) {
      throw (new StateError('Dart -> JavaScript interop not initialised.  Try changing your html to include <script src="packages/browser/inteerop.js"></script>);  Original error: $e'));
    }
    if (jsHasAtob) try {
      return js.context.callMethod('atob', ["$val"]);
    } catch (e) {
      //ignore attepts to decode invalid strings
      //"InvalidCharacterError: Failed to execute 'atob' on 'Window': The string to be decoded is not correctly encoded."
      return null;
    }
    throw (new StateError('Dart -> JavaScript interop not accepting calls to atob().  Try changing your html to include <script src="packages/browser/inteerop.js"></script>);'));
  }

  /// Converts [val] from text to base64.
  ///
  /// if [val] is not valid base64 then returns null
  /// if library dart:js is not working throws StateError
  /// 
  ///    print(MyJS.base64Encode('Hello World'));
  ///    
  static String base64Encode(String val) {
    bool jsHasBtoA;
    try {
      jsHasBtoA = js.context.hasProperty('btoa');
    } catch (e) {
      throw (new StateError('Dart -> JavaScript interop not initialised.  Try changing your html to include <script src="packages/browser/inteerop.js"></script>);  Original error: $e'));
    }
    if (jsHasBtoA) try {
      return js.context.callMethod('btoa', ["$val"]);
    } catch (e) {
      //ignore attepts to decode invalid strings
      //"InvalidCharacterError: Failed to execute 'atob' on 'Window': The string to be decoded is not correctly encoded."
      return null;
    }
    throw (new StateError('Dart -> JavaScript interop not accepting calls to btoa().  Try changing your html to include <script src="packages/browser/inteerop.js"></script>);'));
  }


  ///Returns an exisiting singleton.
  ///
  ///Constructor as invoked by external instanciations.
  factory MyJS() {
    return _singleton;
  }
  
  ///Constructor as invoked by static instances of this class.
  MyJS._initialise() {
  }
}
