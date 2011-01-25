// $Id$

Drupal.Imagecrop.cropUi = Drupal.Imagecrop.cropUi || {};

(function($) { 

$(function () {
  Drupal.Imagecrop.cropUi.initControls();
  Drupal.Imagecrop.cropUi.initScaling();
});  

Drupal.Imagecrop.imageCropWidthField = null;
Drupal.Imagecrop.imageCropHeightField = null;
Drupal.Imagecrop.imageCropXField = null;
Drupal.Imagecrop.imageCropYField = null;
Drupal.Imagecrop.resizeMe = null;

/**
 * Init the controls.
 */
Drupal.Imagecrop.cropUi.initControls = function() {
  
  Drupal.Imagecrop.imageCropWidthField = $('input[name="image-crop-width"]');
  Drupal.Imagecrop.imageCropHeightField = $('input[name="image-crop-height"]');
  Drupal.Imagecrop.imageCropXField = $('input[name="image-crop-x"]');
  Drupal.Imagecrop.imageCropYField = $('input[name="image-crop-y"]');
  
  Drupal.Imagecrop.resizeMe = $('#resizeMe');
  
  if (Drupal.Imagecrop.resizeMe.resizable) { 

    Drupal.Imagecrop.resizeMe.resizable({
      containment: $('#image-crop-container'),
      aspectRatio: Drupal.settings.imagecrop.aspectRatio,
      autohide: true,
      handles: 'n, e, s, w, ne, se, sw, nw',

      resize: function(e, ui) {
        this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
        Drupal.Imagecrop.imageCropWidthField.val(Drupal.Imagecrop.resizeMe.width());
        Drupal.Imagecrop.imageCropHeightField.val(Drupal.Imagecrop.resizeMe.height());
        Drupal.Imagecrop.imageCropXField.val(ui.position.left);
        Drupal.Imagecrop.imageCropYField.val(ui.position.top);
      },
      stop: function(e, ui) {
        this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
      }
    });
    
  }

  Drupal.Imagecrop.resizeMe.draggable({
    cursor: 'move',
    containment: $('#image-crop-container'),
    drag: function(e, ui) {
      this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
      Drupal.Imagecrop.imageCropXField.val(ui.position.left);
      Drupal.Imagecrop.imageCropYField.val(ui.position.top);
    }
  });
  
  $('#image-crop-container').css({ opacity: 0.5 });
  Drupal.Imagecrop.resizeMe.css({ position : 'absolute' });
  
  var leftpos = Drupal.Imagecrop.imageCropXField.val();
  var toppos = Drupal.Imagecrop.imageCropYField.val();
  Drupal.Imagecrop.resizeMe.css({backgroundPosition: '-'+ leftpos + 'px -'+ toppos +'px'});
  Drupal.Imagecrop.resizeMe.width(Drupal.Imagecrop.imageCropWidthField.val() + 'px');
  Drupal.Imagecrop.resizeMe.height($('#edit-image-crop-height').val() + 'px');
  Drupal.Imagecrop.resizeMe.css({top: toppos +'px' });
  Drupal.Imagecrop.resizeMe.css({left: leftpos +'px' });
  
}

/**
 * Init the scaling dropdown.
 */
Drupal.Imagecrop.cropUi.initScaling = function() {
  $('#edit-scaling', '#imagecrop-scale-settings-form').bind('change', Drupal.Imagecrop.cropUi.scaleImage);
}

/**
 * Scale the image to the selected width / height.
 */
Drupal.Imagecrop.cropUi.scaleImage = function() {
  
  var imagecropData = {
      
  }
  
  $.ajax({
    url : Drupal.settings.imagecrop.manipulationUrl,
    data : imagecropData,
    success : function() {
      // force new backgrounds
    }
  })
  
}

})(jQuery); 
