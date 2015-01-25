import 'dart:html';
import 'package:logging/logging.dart' as logging;
import 'package:pappes_web_utility/pappes_web_utility.dart';

void main() {

  logging.hierarchicalLoggingEnabled = true;
  log.level = logging.Level.ALL;
  log.onRecord.listen((logging.LogRecord rec) {
    print('${rec.level.name}: ${rec.time}: ${rec.message}');
  });
  
  
  MyHtml.removeAllOverlays(document);  
}