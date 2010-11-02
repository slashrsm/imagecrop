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
    $function = $module_name .'_imagecrop_popups';
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

  $enabled_styles = array();
  if ($entity_type && $bundle && $field_name) {
    $enabled_styles = imagecrop_get_fields_enabled_styles($entity_type, $field_name, $bundle);
  }

  $styles = get_imagecrop_styles($enabled_styles);
  if (count($styles) == 0) {
    drupal_set_message(t('No preset is found with the javascript_crop action so far. If you want to take advantage of this module, you will need to create at least one preset with that action.'), 'error');
    return '';
  }  

  $file = file_load($fid);
  if (!$file) {
    drupal_set_message(t('The requested file could not be found.'), 'error');
    return '';    
  }
  
  $style_selection = drupal_get_form('imagecrop_style_selection_form', $styles);
  
  return $style_selection;
  
}

/**
 * Show a selection form to switch styles preview.
 */
function imagecrop_style_selection_form($form, $form_state, $styles) {
  
  $form = array();
  $form['styles'] = array(
    '#type' => 'select',
    '#title' => t('Select style to preview'),
    '#options' => $styles,
  );
  
  return $form;
  
}