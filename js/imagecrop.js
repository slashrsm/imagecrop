// $Id$

(function($) { 
  
Drupal.Imagecrop = Drupal.Imagecrop || {};

$(document).ready(function() {
  $("#imagecrop-style-selection-form #edit-styles").change(Drupal.Imagecrop.changeViewedImage);
});

/**
 * Event listener, go to the view url when user selected a style.
 */
Drupal.Imagecrop.changeViewedImage = function() {
  document.location = $("input[name=imagecrop-url]").val().replace('/sid/', '/' + $(this).val() + '/');
}

})(jQuery); 