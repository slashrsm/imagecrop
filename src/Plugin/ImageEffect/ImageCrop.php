<?php

/**
 * @file
 * Contains \Drupal\imagecrop\Plugin\ImageEffect\ImageCrop.
 */

namespace Drupal\imagecrop\Plugin\ImageEffect;

use Drupal\Core\Entity\Query\QueryInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Image\ImageInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\image\ConfigurableImageEffectBase;
use Drupal\imagecrop\ImageCropStorageInterface;
use Psr\Log\LoggerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Uses image crop information to crop an image resource.
 *
 * @ImageEffect(
 *   id = "imagecrop_javascript",
 *   label = @Translation("Javascript crop"),
 *   description = @Translation("Create a crop with a javascript toolbox.")
 * )
 */
class ImageCrop extends ConfigurableImageEffectBase implements ContainerFactoryPluginInterface {

  /**
   * Entity query for imagecrop entity.
   *
   * @var \Drupal\Core\Entity\Query\QueryInterface
   */
  protected $entityQuery;

  /**
   * Image crop storage.
   *
   * @var \Drupal\imagecrop\ImageCropStorageInterface
   */
  protected $storage;

  /**
   * Constructs a Drupal\Component\Plugin\PluginBase object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Psr\Log\LoggerInterface $logger
   *   Logger service.
   * @param \Drupal\Core\Entity\Query\QueryInterface $entity_query
   *   Image crop entity query.
   * @param \Drupal\imagecrop\ImageCropStorageInterface $storage
   *   Image crop storage.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, LoggerInterface $logger, QueryInterface $entity_query, ImageCropStorageInterface $storage) {
    parent::__construct($configuration, $plugin_id, $plugin_definition, $logger);
    $this->entityQuery = $entity_query;
    $this->storage = $storage;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('logger.factory')->get('image'),
      $container->get('entity.query')->get('imagecrop'),
      $container->get('entity_manager')->getStorage('imagecrop')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function applyEffect(ImageInterface $image) {
    // TODO - port this code

    try {
      $imagecrop = new ImageCrop();
      $imagecrop->loadFile($image->source, FALSE);

      // if a global presetid is been set, it meens the image is generated from the imagecrop module
      if (isset($GLOBALS['imagecrop_style'])) {
        $style_name = $GLOBALS['imagecrop_style'];
      }
      // and if not, then get the id from list of all presets
      else {
        $style_name = imagecrop_get_style_name_from_url();
      }

      $imagecrop->setImageStyle($style_name);
      $imagecrop->loadCropSettings();
      $imagecrop->applyCrop($image);

    }
    catch (Exception $e) {
      drupal_set_message(t('Unable to crop image.'), 'error');
      watchdog_exception('imagecrop', $e);
      return FALSE;
    }

    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function transformDimensions(array &$dimensions) {
    // TODO
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return parent::defaultConfiguration() + array(
      'width' => NULL,
      'height' => NULL,
      'xoffset' => 'center',
      'yoffset' => 'center',
      'resizable' => TRUE,
      'downscaling' => FALSE,
      'aspect_ratio' => 'CROP',
      'disable_if_no_data' => TRUE,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = [];

    if (!empty($this->configuration['width']) && !empty($this->configuration['height'])) {
      $form['reset-crops'] = array(
        '#type' => 'checkbox',
        '#title' => t('Reset the already cropped images to the new width and height'),
        '#description' => t('All crop selections that have the same width / height as old settings, will be updated to the new width and height.'),
      );
      $form['old-height'] = array(
        '#type' => 'hidden',
        '#value' => $this->configuration['height'],
      );
      $form['old-width'] = array(
        '#type' => 'hidden',
        '#value' => $this->configuration['width'],
      );
    }

    $form['width'] = [
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $this->configuration['width'],
      '#required' => TRUE,
      '#size' => 10,
      '#element_validate' => [[$this, 'validateSize']],
      '#description' => t('Enter the width in pixels or %'),
    ];

    $form['height'] = [
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => $this->configuration['height'],
      '#required' => TRUE,
      '#size' => 10,
      '#element_validate' => [[$this, 'validateSize']],
      '#description' => t('Enter the height in pixels or %'),
    ];

    $form['xoffset'] = [
      '#type' => 'textfield',
      '#title' => t('X offset'),
      '#default_value' => $this->configuration['xoffset'],
      '#description' => t('Enter an offset in pixels (without px) or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
      '#element_validate' => [[$this, 'validateOffset']],
    ];

    $form['yoffset'] = [
      '#type' => 'textfield',
      '#title' => t('Y offset'),
      '#default_value' => $this->configuration['yoffset'],
      '#description' => t('Enter an offset in pixels (without px) or use a keyword: <em>top</em>, <em>center</em> or <em>bottom</em>.'),
      '#element_validate' => [[$this, 'validateOffset']],
    ];

    $form['resizable'] = [
      '#type' => 'checkbox',
      '#title' => t('Resizable toolbox'),
      '#default_value' => $this->configuration['resizable'],
      '#description' => t('If the toolbox is resized, the crop values won\'t be respected, so you should add a Scale action after the ImageCrop.'),
    ];

    $form['downscaling'] = [
      '#type' => 'checkbox',
      '#title' => t('Do not allow down scaling'),
      '#default_value' => $this->configuration['downscaling'],
      '#description' => t('If checked, you can\'t resize the toolbox smaller than width and height.'),
    ];

    $description = t('Enter an aspect ratio to preserve during resizing. This can take one of the following formats:');
    $description .= '<ul><li>' . t('A float (like 0.5 or 2).') . '</li>';
    $description .= '<li>' . t('The string \'KEEP\'. This will constrain the aspect ratio to that of the original image.') . '</li>';
    $description .= '<li>' . t('The string \'CROP\'. This will constrain the aspect ratio to the dimensions set above.') . '</li></ul>';
    $description .= t('Leave blank for no aspect ratio constraints.');

    $form['aspect_ratio'] = [
      '#type' => 'textfield',
      '#title' => t('Aspect ratio'),
      '#default_value' => $this->configuration['aspect_ratio'],
      '#description' => $description,
      '#element_validate' => [[$this, 'validateAspect']],
    ];

    $form['disable_if_no_data'] = [
      '#type' => 'checkbox',
      '#title' => t('Don\'t crop if cropping region wasn\'t set.'),
      '#default_value' => $this->configuration['disable_if_no_data'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);

    $this->configuration['width'] = $form_state->getValue('width');
    $this->configuration['height'] = $form_state->getValue('height');
    $this->configuration['xoffset'] = $form_state->getValue('yoffset');
    $this->configuration['resizable'] = $form_state->getValue('resizable');
    $this->configuration['downscaling'] = $form_state->getValue('downscaling');
    $this->configuration['aspect_ratio'] = $form_state->getValue('disable_if_no_data');

    if ($form_state->getValue('width')) {
      $ids = $this->entityQuery
        ->condition('width', $form_state->getValue('old-width'))
        ->condition('height', $form_state->getValue('old-height'))
        // TODO
        //->condition('image_style', )
        ->execute();

      $image_crops = $this->storage->loadMultiple($ids);
      /** @var \Drupal\imagecrop\ImageCropInterface $crop */
      foreach ($image_crops as $crop) {
        $crop->set('width', $form_state->getValue('width'));
        $crop->set('height', $form_state->getValue('height'));
        $crop->set('revision_log', t('Updated when resetting crop dimensions.'));
        $crop->save();
      }
    }
  }

  /**
   * Validate the entered height / width. (can be procent or numeric)
   */
  protected function validateSize($element, FormStateInterface &$form_state) {
    $value = str_replace('%', '', $element['#value']);
    if ($value != '' && (!is_numeric($value) || intval($value) <= 0)) {
      $form_state->setError($element, t('!name must be a correct size.', ['!name' => $element['#title']]));
    }
  }

  /**
   * Validation function to validate an entered offset value. (numbers or left / center / right)
   */
  function validateOffset($element, FormStateInterface &$form_state) {
    if ($element['#value'] == '' || is_numeric($element['#value'])) {
      return;
    }

    if ($element['#name'] == 'data[yoffset]') {
      $allowed_values = ['center', 'top', 'bottom'];
    }
    else {
      $allowed_values = ['center', 'left', 'right'];
    }

    // if the value is a string, check on allowed strings
    if (!in_array($element['#value'], $allowed_values)) {
      $form_state->setError($element, t('@name must be a correct offset value', ['@name' => $element['#title']]));
    }
  }

  /**
   * Validation function to validate an entered aspect value.
   */
  function validateAspect($element, FormStateInterface &$form_state) {
    if ($element['#value'] == '' || is_numeric($element['#value'])) {
      return;
    }

    // if the value is a string, check on allowed strings
    if ($element['#value'] != 'KEEP' && $element['#value'] != 'CROP') {
      $form_state->setError($element, t('@name must be a correct aspect value', ['@name' => $element['#title']]));
    }
  }
}
