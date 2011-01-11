// $Id$

Drupal.Imagecrop.cropUi = Drupal.Imagecrop.cropUi || {};

(function($) { 

$(function () {
  Drupal.Imagecrop.cropUi.initControls();
});  

Drupal.Imagecrop.imageCropWidthField = null;
Drupal.Imagecrop.imageCropHeightField = null;
Drupal.Imagecrop.imageCropXField = null;
Drupal.Imagecrop.imageCropYField = null;

/**
 * Init the controls.
 */
Drupal.Imagecrop.cropUi.initControls = function() {
  
  Drupal.Imagecrop.imageCropWidthField = $('input[name="image-crop-width"]');
  Drupal.Imagecrop.imageCropHeightField = $('input[name="image-crop-height"]');
  Drupal.Imagecrop.imageCropXField = $('input[name="image-crop-x"]');
  Drupal.Imagecrop.imageCropYField = $('input[name="image-crop-y"]');
  
  if ($('#resizeMe').resizable) { 

    $('#resizeMe').resizable({
      containment: $('#image-crop-container'),
      aspectRatio: Drupal.settings.imagecrop.aspectRatio,
      autohide: true,
      handles: 'n, e, s, w, ne, se, sw, nw',

      resize: function(e, ui) {
        this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
        Drupal.Imagecrop.imageCropWidthField.val($('#resizeMe').width());
        Drupal.Imagecrop.imageCropHeightField.val($('#resizeMe').height());
        Drupal.Imagecrop.imageCropXField.val(ui.position.left);
        Drupal.Imagecrop.imageCropYField.val(ui.position.top);
      },
      stop: function(e, ui) {
        this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
      }
    });
    
  }

  $('#resizeMe').draggable({
    cursor: 'move',
    containment: $('#image-crop-container'),
    drag: function(e, ui) {
      this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
      $("#edit-image-crop-x").val(ui.position.left);
      $("#edit-image-crop-y").val(ui.position.top);
    }
  });
  
  $('#image-crop-container').css({ opacity: 0.5 });
  $('#resizeMe').css({ position : 'absolute' });
  
    var leftpos = $('#edit-image-crop-x').val();
    var toppos = $('#edit-image-crop-y').val();
    $("#resizeMe").css({backgroundPosition: '-'+ leftpos + 'px -'+ toppos +'px'});
    $("#resizeMe").width($('#edit-image-crop-width').val() + 'px');
    $("#resizeMe").height($('#edit-image-crop-height').val() + 'px');
    $("#resizeMe").css({top: $('#edit-image-crop-y').val() +'px' });
    $("#resizeMe").css({left: $('#edit-image-crop-x').val() +'px' });
  
  
}

})(jQuery); 
