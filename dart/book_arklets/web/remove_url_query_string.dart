import 'dart:html';
import 'package:pappes_web_utility/pappes_web_utility.dart';
import 'logging_functions.dart';

void main() {
  //use common function to set up logging
  turnOnLogging();
  //change all href Elements in the body to have blank UriParameters
  MyHtml.iterateHTMLDOM(
      document.body, 
      (element) => MyHtml.alterAttribute(
          element, 
          'href', 
          (url) => MyHtml.setUriParameters(url)));  
}