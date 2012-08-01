(function ($) {

  Drupal.Imagecrop = Drupal.Imagecrop || {};
  Drupal.Imagecrop.dialogContainer = null;
  
  /**
   * Implement Drupal behavior for autoattach
   */
  Drupal.behaviors.imagecropDialog = {
    attach: function(context) {
      // Add dialog listener on imagecrop links.
      $(context).find('a.imagecrop-button').bind('click', Drupal.Imagecrop.openDialog);
    }
  }
  
  /**
   * Click listener: Open the dialog for clicked link.
   */
  Drupal.Imagecrop.openDialog = function(e) {
    
    e.preventDefault();
    var page = $(this).attr("href");
    var pagetitle = $(this).attr("title");
    Drupal.Imagecrop.dialogContainer = $('<div id="imagecrop-modal-container"></div>')
      .html('<iframe style="border: 0px; " src="' + page + '" width="100%" height="100%"></iframe>')
      .dialog({
        autoOpen: true,
        closeOnEscape : false,
        dialogClass : 'imagecrop-dialog',
        modal: true,
        height: Drupal.settings.imagecrop.popupHeight,
        width: Drupal.settings.imagecrop.popupWidth,
        title: pagetitle
      });
    
  }
  
})(jQuery);