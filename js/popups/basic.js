(function($) { 

$(document).ready(function() {
  $('#cancel-crop').click(Drupal.Imagecrop.closePopup);
});

/**
 * Event listener, close the current popup.
 */
Drupal.Imagecrop.closePopup = function() {
  window.close();
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