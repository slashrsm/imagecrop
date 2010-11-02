<?php
// $Id$

/**
 * @file
 * Registry file for Imagecrop
 *
 * @date
 * Nov 2, 2010
 */

/**
 * Implements hook_perm().
 */
function imagecrop_perm() {
  return array('crop any image with toolbox', 'crop images with toolbox', 'administer imagecrop');
}

/**
 * Implements hook_theme().
 */
function imagecrop_theme() {

  return array(
    'page-imagecrop' => array(
      'variables' => array(),
      'file' => 'imagecrop.theme.inc',
      'function' => 'imagecrop_page',
    ),
    'html-imagecrop' => array(
      'variables' => array(),
      'template' => 'html-imagecrop',
    ),
    'imagecrop_javascript' => array(
      'variables' => array('element' => NULL),
      'file' => 'imagecrop.theme.inc',
    ),
    'imagecrop' => array(
      'variables' => array('url' => NULL, 'width' => NULL, 'height' => NULL, 'resize' => NULL),
      'file' => 'imagecrop.theme.inc',
    ),
    'imagecrop_result' => array(
      'variables' => array('presetname' => NULL, 'filepath' => NULL, 'alt' => NULL, 'attributes' => NULL),
      'file' => 'imagecrop.theme.inc',
    ),
    'presettabs' => array(
      'variables' => array('presets' => array(), 'fid' => NULL, 'presetid' => NULL, 'module' => NULL, 'field' => NULL, 'node_type' => NULL),
      'file' => 'imagecrop.theme.inc',
    ),
  );

}

/**
 * Implements hook_menu().
 */
function imagecrop_menu() {

  $items = array();

  $items['admin/config/media/imagecrop'] = array(
    'title' => 'Image javascript crop',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('imagecrop_settings_form'),
    'access arguments' => array('administer imagecrop'),
    'file' => 'imagecrop.admin.inc',
  );

  $items['imagecrop/overview/%file'] = array(
    'page callback' => 'imagecrop_image_styles_overview',
    'page arguments' => array(2),
    'type' => MENU_CALLBACK,
    'access arguments' => array('crop images with toolbox'),
    'file' => 'imagecrop.admin.inc',
  );

  $items['imagecrop/crop/%file/%'] = array(
    'page callback' => 'imagecrop_select_crop',
    'page arguments' => array(2, 3),
    'type' => MENU_CALLBACK,
    'access arguments' => array('crop images with toolbox'),
    'file' => 'imagecrop.admin.inc',
  );

  return $items;

}

/**
 * Implements hook_image_effect_info().
 */
function imagecrop_image_effect_info() {

  $effects = array();

  $effects['imagecrop_javascript'] = array(
    'label' => t('Javascript crop'),
    'help' => t('Create a crop with a javascript toolbox.'),
    'effect callback' => 'imagecrop_effect',
    'form callback' => 'imagecrop_effect_form',
    'summary theme' => 'imagecrop_effect_summary',
  );

  return $effects;

}