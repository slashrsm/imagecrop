// $Id$

Drupal.Imagecrop = Drupal.Imagecrop || {};

(function($) { 

$(document).ready(function() {
  $("#imagecrop-style-selection-form #edit-styles").change(Drupal.Imagecrop.changeViewedImage);
});

/**
 * Event listener, go to the view url when user selected a style.
 */
Drupal.Imagecrop.changeViewedImage = function() {
  document.location = $("input[name=imagecrop-url]").val().replace('/isid/', '/' + $(this).val() + '/');
}

})(jQuery); 