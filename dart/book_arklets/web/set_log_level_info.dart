// Copyright (c) 2015, Pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.


import 'package:logging/logging.dart' as logging;
import 'package:pappes_web_utility/pappes_web_utility.dart';

main() {

  logging.hierarchicalLoggingEnabled = true;
  log.level = logging.Level.INFO;
  log.onRecord.listen((logging.LogRecord rec) {
    print('${rec.level.name}: ${rec.time}: ${rec.message}');
  });
}
