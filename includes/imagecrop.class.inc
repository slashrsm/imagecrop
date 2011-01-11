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
  private $inCroppingMode = FALSE;

  private $file;
  private $imageStyle;
  private $entityType;
  private $bundle;
  private $fieldName;

  private $isResizable = FALSE;
  private $resizeAspectRatio = FALSE;
  private $width = 0;
  private $height = 0;
  private $xoffset = 0;
  private $disableIfNoData = FALSE;

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

    $this->file->filepath = file_create_url($this->file->uri);

  }

  /**
   * Get the current file.
   */
  public function getFile() {
  	return $this->file;
  }

  /**
   * Set the entity type from the current imagecrop.
   */
  public function setEntityType($entity_type) {
  	$this->entityType = $entity_type;
  }

 /**
   * Get the entity type from the current imagecrop.
   */
  public function getEntityType() {
    return $this->entityType;
  }

  /**
   * Set the bundle from the current imagecrop.
   */
  public function setBundle($bundle) {
    $this->bundle = $bundle;
  }

  /**
   * Get the bundle from the current imagecrop.
   */
  public function getBundle() {
    return $this->bundle;
  }

  /**
   * Set the field name from the current imagecrop.
   */
  public function setFieldName($field_name) {
    $this->fieldName = $field_name;
  }

  /**
   * Get the field name from the current imagecrop.
   */
  public function getFieldName() {
    return $this->fieldName;
  }

  /**
   * Get the X offset from the current imagecrop object.
   */
  public function getXOffset() {

  }

  /**
   * Get the X offset from the current imagecrop object.
   */
  public function getYOffset() {

  }

  /**
   * Get the width from the current crop area.
   */
  public function getWidth() {
    return $this->width;
  }

  /**
   * Get the height from the current crop area.
   */
  public function getHeight() {
    return $this->height;
  }

  /**
   * Get the width from the image to crop.
   */
  public function getImageWidth() {
    return $this->imageWidth;
  }

  /**
   * Get the height from the image to crop.
   */
  public function getImageHeight() {
    return $this->imageHeight;
  }

  /**
   * Set the status of cropping mode (TRUE = busy cropping).
   */
  public function setInCroppingMode($inCroppingMode) {
  	$this->inCroppingMode = $inCroppingMode;
  }

  /**
   * Get the current value for cropping mode.
   */
  public function getInCroppingMode() {
  	return $this->inCroppingMode;
  }

  /**
   * Set the current cropped image style.
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
        $this->resizeAspectRatio = $effect['data']['aspect_ratio'];
        break;
      }
    }

  }

  /**
   * Get the current cropped image style.
   */
  public function getImageStyle() {
    return $this->imageStyle;
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

    $size = getimagesize($this->file->filepath);
    $this->imageWidth = $size[0];
    $this->imageHeight = $size[1];

  }

  /**
   * Add all the files for the cropping UI.
   */
  public function addImagecropUi() {

    drupal_add_js(drupal_get_path('module', 'imagecrop') .'/js/imagecrop.js');
    drupal_add_css(drupal_get_path('module', 'imagecrop') .'/imagecrop.css');

    // Add crop ui if in cropping mode.
    if ($this->inCroppingMode) {

      drupal_add_js(drupal_get_path('module', 'imagecrop') .'/js/imagecrop.ui.crop.js');

      drupal_add_library('system', 'ui.draggable');

      if ($this->isResizable) {
        drupal_add_library('system', 'ui.resizable');
        drupal_add_library('system', 'effects.scale');
        drupal_add_js(array('imagecrop' => array('resizeAspectRatio' => $this->resizeAspectRatio)), 'setting');
      }

    }

  }

}