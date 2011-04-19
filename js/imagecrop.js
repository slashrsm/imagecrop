
Drupal.Imagecrop = Drupal.Imagecrop || {};
Drupal.Imagecrop.hasUnsavedChanges = false;

(function($) { 

$(document).ready(function() {
  
  $("#imagecrop-style-selection-form #edit-styles").change(function() { Drupal.Imagecrop.changeViewedImage($(this).val()); });
  if (Drupal.settings.imagecrop.cropped) {
    Drupal.Imagecrop.forceUpdate();
    $('#cancel-crop').html(Drupal.t('Done cropping'));
  }
  
});

/**
 * Event listener, go to the view url when user selected a style.
 */
Drupal.Imagecrop.changeViewedImage = function(isid) {
  document.location = $("input[name=imagecrop-url]").val().replace('/isid/', '/' + isid + '/');
}

/**
 * Force an update from the imagefield widgets.
 */
Drupal.Imagecrop.forceUpdate = function() {
  $('.image-preview img', window.opener.document).each(function() {
    var source = $(this).attr('src');
    $(this).attr('src', (source + '?time=' + new Date().getTime()));
  });
}

})(jQuery); 