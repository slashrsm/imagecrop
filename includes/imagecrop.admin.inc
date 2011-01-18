<?php
// $Id$

/**
 * @file
 * Administration tools for the imagecrop module.
 *
 * @date
 * Oct 7, 2010
 */

/**
 * General settings form from the imagecrop module.
 */
function imagecrop_settings_form() {

  $all_popups = array('basic' => t('Basic popup window'));
  foreach (module_implements('imagecrop_popups') as $module_name) {
    $function = $module_name . '_imagecrop_popups';
    $popups = $function();
    if (is_array($popups)) {
      $all_popups += $popups;
    }
  }

  $form['imagecrop_popup'] = array(
    '#type' => 'radios',
    '#title' => t('Popup window type'),
    '#default_value' => variable_get('imagecrop_popup', 'basic'),
    '#options' => $all_popups,
  );

  $form['imagecrop_popup_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Popup window width'),
    '#default_value' => variable_get('imagecrop_popup_width', 700),
    '#size' => 4,
    '#field_suffix' => 'pixels',
  );

  $form['imagecrop_popup_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Popup window height'),
    '#default_value' => variable_get('imagecrop_popup_height', 600),
    '#size' => 4,
    '#field_suffix' => 'pixels',
  );

  $form['imagecrop_scale_step'] = array(
    '#type' => 'textfield',
    '#title' => t('Step size for scale dropdown'),
    '#default_value' => variable_get('imagecrop_scale_step', 50),
    '#size' => 4,
    '#field_suffix' => 'pixels',
  );

  // drupal message if no action is found with javascript_crop
  if (count(get_imagecrop_styles()) == 0) {
    drupal_set_message(t('No image style is found with the javascript_crop action so far. If you want to take advantage of this module, you will need to create at least one image style with that action.'));
  }

  return system_settings_form($form);

}

/**
 * Show an overview from the enabled image styles for this file.
 * Show an example from the first (or selected) image.
 *
 * @param $fid fid from file to be viewed
 * @param $isid Style id that default needs to be shown
 * @param $entity_type entity type from the field that triggered the imagecrop overview
 * @param $bundle bundle from the field that triggered the imagecrop overview
 * @param $field_name Field name that triggered the imagecrop overview.
 */
function imagecrop_image_styles_overview($fid, $isid, $entity_type = NULL, $bundle = NULL, $field_name = NULL) {

  try {

    $styles = imagecrop_get_enabled_styles_for_crop($entity_type, $bundle, $field_name);

    $style_name = current($styles);
    if ($isid > 0) {
      if (!isset($styles[$isid])) {
        throw new Exception('The image style to crop was not found.');
      }
      $style_name = $styles[$isid];
    }
    else {
      $isid = key($styles);
    }

    global $user;
    $imagecrop = new ImageCrop();
    $imagecrop->loadFile($fid);

    global $user;
    if (!$imagecrop->hasUserAccess($user)) {
      watchdog('imagecrop', 'Access denied for user @name on fid @fid', array('@name' => $user->name, '@fid' => $imagecrop->file->fid));
      throw new Exception("You don't have permissions to crop this image.");
    }

    $imagecrop->setImageStyle($isid);
    $imagecrop->setEntityType($entity_type);
    $imagecrop->setBundle($bundle);
    $imagecrop->setfieldName($field_name);
    $imagecrop->addImagecropUi();

    $style_selection = drupal_render(drupal_get_form('imagecrop_style_selection_form', $styles, $imagecrop));
    $style_selection .= theme('image_style', array(
      'style_name' => $style_name,
      'path' => $imagecrop->getFile()->uri,
    ));

    return $style_selection;

  }
  catch (Exception $e) {
    drupal_set_message(check_plain(t($e->getMessage())), 'error');
    return '';
  }

}

/**
 * Show the cropping page for the selected image style.
 *
 * @param $fid fid from file to be cropped
 * @param $isid Style id that is being changed
 * @param $entity_type entity type from the field that triggered the imagecrop overview
 * @param $bundle bundle from the field that triggered the imagecrop overview
 * @param $field_name Field name that triggered the imagecrop overview.
 */
function imagecrop_cropping_page($fid, $isid, $entity_type = NULL, $bundle = NULL, $field_name = NULL) {

  try {

    $styles = imagecrop_get_enabled_styles_for_crop($entity_type, $bundle, $field_name);

    global $user;
    $imagecrop = new ImageCrop();
    $imagecrop->loadFile($fid);
    $imagecrop->setImageStyle($isid);
    $imagecrop->setEntityType($entity_type);
    $imagecrop->setBundle($bundle);
    $imagecrop->setFieldName($field_name);
    $imagecrop->setInCroppingMode(TRUE);

    global $user;
    if (!$imagecrop->hasUserAccess($user)) {
      watchdog('imagecrop', 'Access denied for user @name on fid @fid', array('@name' => $user->name, '@fid' => $imagecrop->file->fid));
      throw new Exception("You don't have permissions to crop this image.");
    }

    $imagecrop->loadCropSettings();
    $imagecrop->addImagecropUi();

    return theme('imagecrop_crop_ui', array(
      'style_selection' => drupal_get_form('imagecrop_style_selection_form', $styles, $imagecrop),
      'settings_form' => drupal_get_form('imagecrop_crop_settings_form', $imagecrop),
      'imagecrop' => $imagecrop,
    ));

  }
  catch (Exception $e) {
    drupal_set_message(check_plain(t($e->getMessage())), 'error');
    return '';
  }

}

/**
 * Show a selection form to switch styles preview.
 */
function imagecrop_style_selection_form($form, $form_state, $styles, $imagecrop) {

  $image_style = $imagecrop->getImageStyle();

  $form = array();
  $form['styles'] = array(
    '#type' => 'select',
    '#title' => t('Select style to preview'),
    '#options' => $styles,
    '#default_value' => $image_style['isid'],
  );

  $path = $imagecrop->getFile()->fid . '/isid/' . $imagecrop->getEntityType() . '/' . $imagecrop->getBundle() . '/' . $imagecrop->getFieldName();
  $form['imagecrop-url'] = array(
    '#type' => 'hidden',
    '#value' => url('imagecrop/overview/' . $path, array('absolute' => TRUE)),
  );

  if (!$imagecrop->getInCroppingMode()) {
    $form['#suffix'] = l(t('Edit this style'), 'imagecrop/crop/' . str_replace('/isid/', '/' . $image_style['isid'] . '/', $path));
  }

  return $form;

}

/**
 * Settings form from the current crop.
 */
function imagecrop_crop_settings_form($form, $form_state, $imagecrop) {

  $form['image-crop-x'] = array(
    '#type' => 'hidden',
    '#default_value' => $imagecrop->getXOffset(),
    '#attributes' => array('class' => 'edit-image-crop-x'),
  );

  $form['image-crop-y'] = array(
    '#type' => 'hidden',
    '#default_value' => $imagecrop->getYOffset(),
  );

  $form['image-crop-width'] = array(
    '#type' => 'hidden',
    '#default_value' => $imagecrop->getWidth(),
    '#attributes' => array('class' => 'edit-image-crop-width'),
  );

  $form['image-crop-height'] = array(
    '#type' => 'hidden',
    '#default_value' => $imagecrop->getHeight(),
    '#attributes' => array('class' => 'edit-image-crop-height'),
  );

  $form['fid'] = array(
    '#type' => 'hidden',
    '#value' => $imagecrop->getFile()->fid,
  );

  $form['field'] = array(
    '#type' => 'hidden',
    '#value' => serialize(array($imagecrop->getEntityType(), $imagecrop->getBundle(), $imagecrop->getFieldName())),
  );

  $image_style = $imagecrop->getImageStyle();

  $form['isid'] = array(
    '#type' => 'hidden',
    '#value' => $image_style['isid'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Crop image thumbnail'),
  );

  return $form;

}

/**
 * Submit the settings form. Save all cropping settings.
 */
function imagecrop_crop_settings_form_submit($form, &$form_state) {

  db_delete('image_crop_settings')
    ->condition('fid', $form_state['values']['fid'])
    ->condition('isid', $form_state['values']['isid'])
  ->execute();

  $record = new stdClass();
  $record->fid = $form_state['values']['fid'];
  $record->isid = $form_state['values']['isid'];
  $record->xoffset = $form_state['values']['image-crop-x'];
  $record->yoffset = $form_state['values']['image-crop-y'];
  $record->width = $form_state['values']['image-crop-width'];
  $record->height = $form_state['values']['image-crop-height'];

  drupal_write_record('image_crop_settings', $record);

  $field_info = unserialize($form_state['values']['field']);
  // delete imagecache preset, so newest file is generated.
/*  file_delete($form_state['values']['preset-destination']);
  if ($form_state['values']['preset-destination'] != $form_state['values']['temp-preset-destination']) {
    file_delete($form_state['values']['temp-preset-destination']);
  }*/

  drupal_goto('imagecrop/overview/' . $record->fid . '/' . $record->isid . '/' . $field_info[0] . '/' . $field_info[1] . '/' . $field_info[2]);

}

/**
 * Get all the enabled styles for the current crop url.
 */
function imagecrop_get_enabled_styles_for_crop($entity_type, $bundle, $field_name) {

  $enabled_styles = array();
  if ($entity_type && $bundle && $field_name) {
    $enabled_styles = imagecrop_get_fields_enabled_styles($entity_type, $field_name, $bundle);
  }

  $styles = get_imagecrop_styles($enabled_styles);
  if (count($styles) == 0) {
    throw new Exception('No preset is found with the javascript_crop action so far. If you want to take advantage of this module, you will need to create at least one preset with that action.');
  }

  return $styles;

}