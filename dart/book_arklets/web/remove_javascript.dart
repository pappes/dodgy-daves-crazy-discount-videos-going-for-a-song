import 'dart:html';
import 'package:pappes_web_utility/pappes_web_utility.dart';
import 'logging_functions.dart';

void main() {
  //use common function to set up logging
  turnOnLogging();
  //have an attack on all scripts in the page
  MyHtml.removeAllScripts(document);  
}