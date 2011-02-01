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
Drupal.Imagecrop.hasUnsavedChanges = false;

/**
 * Init the controls.
 */
Drupal.Imagecrop.cropUi.initControls = function() {
  
  Drupal.Imagecrop.imageCropWidthField = $('input[name="image-crop-width"]', '#imagecrop-crop-settings-form');
  Drupal.Imagecrop.imageCropHeightField = $('input[name="image-crop-height"]', '#imagecrop-crop-settings-form');
  Drupal.Imagecrop.imageCropXField = $('input[name="image-crop-x"]', '#imagecrop-crop-settings-form');
  Drupal.Imagecrop.imageCropYField = $('input[name="image-crop-y"]', '#imagecrop-crop-settings-form');
  
  Drupal.Imagecrop.resizeMe = $('#resizeMe');
  
  if (Drupal.Imagecrop.resizeMe.resizable) { 

    Drupal.Imagecrop.resizeMe.resizable({
      containment: $('#image-crop-container'),
      aspectRatio: Drupal.settings.imagecrop.aspectRatio,
      autohide: true,
      handles: 'n, e, s, w, ne, se, sw, nw',

      resize: function(e, ui) {
        Drupal.Imagecrop.hasUnsavedChanges = true;
        this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
        Drupal.Imagecrop.imageCropWidthField.val(Drupal.Imagecrop.resizeMe.width());
        Drupal.Imagecrop.imageCropHeightField.val(Drupal.Imagecrop.resizeMe.height());
        Drupal.Imagecrop.imageCropXField.val(ui.position.left);
        Drupal.Imagecrop.imageCropYField.val(ui.position.top);
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
      Drupal.Imagecrop.hasUnsavedChanges = true;
    }
  });
  
  $('#image-crop-container').css({ opacity: 0.5 });
  Drupal.Imagecrop.resizeMe.css({ position : 'absolute' });
  
  var leftpos = Drupal.Imagecrop.imageCropXField.val();
  var toppos = Drupal.Imagecrop.imageCropYField.val();
  Drupal.Imagecrop.resizeMe.css({backgroundPosition: '-'+ leftpos + 'px -'+ toppos +'px'});
  Drupal.Imagecrop.resizeMe.width(Drupal.Imagecrop.imageCropWidthField.val() + 'px');
  Drupal.Imagecrop.resizeMe.height($('#edit-image-crop-height', '#imagecrop-crop-settings-form').val() + 'px');
  Drupal.Imagecrop.resizeMe.css({top: toppos +'px' });
  Drupal.Imagecrop.resizeMe.css({left: leftpos +'px' });
  
}

/**
 * Init the scaling dropdown.
 */
Drupal.Imagecrop.cropUi.initScaling = function() {
  
  Drupal.Imagecrop.fid = $('input[name="fid"]', '#imagecrop-crop-settings-form').val();
  Drupal.Imagecrop.isid = $('input[name="isid"]', '#imagecrop-crop-settings-form').val();
  Drupal.Imagecrop.cropFile = $('input[name="temp-style-destination"]', '#imagecrop-crop-settings-form').val();
  $('#edit-scaling', '#imagecrop-scale-settings-form').bind('change', Drupal.Imagecrop.cropUi.scaleImage);
  Drupal.Imagecrop.cropUi.cropContainer = $('#image-crop-container');
  Drupal.Imagecrop.cropUi.cropWrapper = $('#imagecrop-crop-wrapper');
  
}

/**
 * Scale the image to the selected width / height.
 */
Drupal.Imagecrop.cropUi.scaleImage = function() {
  
  var dimensions = $(this).val().split('x');
  if (dimensions.length != 2) {
    return false;
  }
  
  var imagecropData = {
    'fid' : Drupal.Imagecrop.fid,
    'isid' : Drupal.Imagecrop.isid,
    'scale' : dimensions[0]
  }
  
  $.ajax({
    url : Drupal.settings.imagecrop.manipulationUrl,
    data : imagecropData,
    type : 'post',
    success : function() {
      
      Drupal.Imagecrop.hasUnsavedChanges = true;
      
      // force new backgrounds and width / height
      var background = Drupal.Imagecrop.cropFile + '?time=' +  new Date().getTime();
      Drupal.Imagecrop.cropUi.cropContainer.css({
        'background-image' : 'url(' + background + ')',
        'width' : dimensions[0],
        'height' : dimensions[1]
      });

      Drupal.Imagecrop.cropUi.cropWrapper.css({
        'width' : dimensions[0],
        'height' : dimensions[1]
      });      
      
      // make resize smaller when new image is smaller
      if (Drupal.Imagecrop.resizeMe.height() > dimensions[1]) {
        Drupal.Imagecrop.resizeMe.height(dimensions[1]);
      }
      if (Drupal.Imagecrop.resizeMe.width() > dimensions[0]) {
        Drupal.Imagecrop.resizeMe.width(dimensions[0]);
      }      
      
    }
  })
  
}

})(jQuery); 
