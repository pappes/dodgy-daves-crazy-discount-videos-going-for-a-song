// Copyright (c) 2015, Pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.


import 'package:logging/logging.dart' as logging;
import 'package:pappes_web_utility/pappes_web_utility.dart';
import 'dart:html';
import 'dart:convert';

setLogLevel(logging.Level level) {
  HiddenInputElement logLevelElement = querySelector('#pappes_web_utility.log_level');
  if (logLevelElement==null) {
    logLevelElement=new HiddenInputElement()
      ..name='pappes_web_utility.log_level';
    document.insertBefore(document.nodes.first, logLevelElement);
  }
  //convert object [logging.Level] to JSON to store in the HTML page
  var mapData = new Map();
  mapData["name"] = level.name;
  mapData["value"] = level.value;
  logLevelElement.value=JSON.encode(mapData);
}

turnOnLogging() {
  HiddenInputElement logLevelElement = querySelector('#pappes_web_utility.log_level');
  if (logLevelElement!=null) {
    logging.hierarchicalLoggingEnabled = true;
    log.onRecord.listen((logging.LogRecord rec) {
      print('${rec.level.name}: ${rec.time}: ${rec.message}');
      MyHtml.logMessageIntoHTMLBodyComment('${rec.level.name}: ${rec.time}: ${rec.message}');

    });
    //convert JSON to object [logging.Level]
    List parsedList = JSON.decode(logLevelElement.value);
    log.level = new logging.Level(parsedList[0], parsedList[1]);
  }
}
