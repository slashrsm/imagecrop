<?php
// $Id$

/**
 * @file
 * Imagecrop class to handle the javascript imagecrop.
 *
 * @date
 * Dec 19, 2010
 */

class ImageCrop {

  private $allowedExtensions = array('image/jpeg', 'image/gif', 'image/png', 'image/pjpeg');
  private $file = null;
  private $imageStyle = null;
  private $inCroppingMode = FALSE;

  /**
   * Load the imagecrop settings for the given fid.
   */
  public function loadFile($fid) {

    $this->file = file_load($fid);

    if (!$this->file) {
    	throw new Exception('The image to crop was not found.');
    }

    if (!in_array($this->file->filemime, $this->allowedExtensions)) {
    	throw new Extension('The file to crop was not an image.');
    }

  }

  /**
   * Get the current file.
   */
  public function getFile() {
  	return $this->file;
  }

  /**
   * Setter for status of cropping mode.
   */
  public function setInCroppingMode($inCroppingMode) {
  	$this->inCroppingMode = $inCroppingMode;
  }

  /**
   * Set the current active image style.
   */
  public function setImageStyle($isid) {

    $this->imageStyle = image_style_load(NULL, $isid);
    if (!$this->imageStyle) {
      throw new Exception('The image style to crop was not found.');
    }

    // add default settings
    foreach ($this->imageStyle['effects'] as $effect) {
      if ($effect['name'] == 'imagecrop_javascript') {
        $this->width = $effect['data']['width'];
        $this->height = $effect['data']['height'];
        $this->xoffset = $effect['data']['xoffset'];
        $this->yoffset = $effect['data']['yoffset'];
        $this->isResizable = $effect['data']['resizable'];
        $this->disableIfNoData = $effect['data']['disable_if_no_data'];
        break;
      }
    }

  }

  /**
   * Check if a user has permissions to the current file.
   */
  public function hasUserAccess($account) {

    // access to all images
    if (user_access('crop any image with toolbox')) {
      return TRUE;
    }

    // If not administer nodes, check if the files uid is the same like users uid.
    if (!user_access('administer nodes') && $account->uid != $this->file->uid) {
      return FALSE;
    }

    return TRUE;

  }

  /**
   * Load the crop settings that are available.
   */
  public function loadCropSettings() {


  }

  /**
   * Add all the files for the cropping UI.
   */
  public function addImagecropUi() {

    drupal_add_js(drupal_get_path('module', 'imagecrop') .'/js/imagecrop.js');

    // Add crop ui if in cropping mode.
    if ($this->inCroppingMode) {

      drupal_add_js(drupal_get_path('module', 'imagecrop') .'/js/imagecrop.ui.crop.js');

      drupal_add_library('system', 'ui.draggable');

      if ($this->isResizable) {
        drupal_add_library('system', 'ui.resizable');
        drupal_add_library('system', 'effects.scale');
      }

    }

  }

  public function getCropArea() {

    $uri = file_create_url($this->file->uri);

    $output = '
    <div style="margin-left: 3px;"><div class="imagefield-crop-wrapper" id="imagefield-crop-wrapper" style="position: absolute; margin-top: 45px; width: '. $this->width .'px; height: '. $this->height .'px;">
      <div id="image-crop-container" style="background-image: url(\''. $uri .'\'); width:'. $this->width  .'px; height:'. $this->height  .'px;"></div>
      <div id="resizeMe" style="background-image: url(\''. $uri .'\'); width:'. $this->width  .'px; height:'. $this->height  .'px; top: 20px;">';
    $output .= '</div></div></div><div style="clear:both;"></div>';

    return $output;

  }

}