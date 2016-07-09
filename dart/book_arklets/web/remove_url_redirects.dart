import 'dart:html';
import 'package:pappes_web_utility/pappes_web_utility.dart';
import 'logging_functions.dart';

void main() {
  //use common function to set up logging
  turnOnLogging();
  //change all href Elements in the body to point directly to the target page
  MyHtml.iterateHTMLDOM(
      document.body, 
      (element) => MyHtml.alterAttribute(
          element, 
          'href', 
          (url) => MyHtml.removeUrlRedirect(url)));  
}