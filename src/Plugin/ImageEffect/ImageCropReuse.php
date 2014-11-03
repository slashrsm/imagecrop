<?php

/**
 * @file
 * Contains \Drupal\imagecrop\Plugin\ImageEffect\ImageCrop.
 */

namespace Drupal\imagecrop\Plugin\ImageEffect;

use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\Query\QueryInterface;
use Drupal\Core\Entity\Sql\SqlEntityStorageInterface;
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
 *   id = "imagecrop_reuse",
 *   label = @Translation("Reuse a javascript crop selection"),
 *   description = @Translation("Reuse crop selection from another javascript crop preset.")
 * )
 */
class ImageCropReuse extends ConfigurableImageEffectBase implements ContainerFactoryPluginInterface {

  /**
   * Image style storage.
   *
   * @var \Drupal\Core\Entity\Query\QueryInterface
   */
  protected $styleStorage;

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
  public function __construct(array $configuration, $plugin_id, $plugin_definition, LoggerInterface $logger, EntityStorageInterface $style_storage, ImageCropStorageInterface $storage) {
    parent::__construct($configuration, $plugin_id, $plugin_definition, $logger);
    $this->styleStorage = $style_storage;
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
      $container->get('entity_manager')->getStorage('image_style'),
      $container->get('entity_manager')->getStorage('imagecrop')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function applyEffect(ImageInterface $image) {
    // TODO - port this code

    if (empty($this->configuration['image_style'])) {
      return FALSE;
    }

    // Load selected image style and apply the imagecrop_javascript action.
    $style = $this->styleStorage->load($this->configuration['image_style']);
    foreach ($style['effects'] as $effect) {
      if ($effect['name'] == 'imagecrop_javascript') {
        $GLOBALS['imagecrop_style'] = $style['name'];
        image_effect_apply($image, $effect);
        return TRUE;
      }
    }

    return FALSE;

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
      'image_style' => NULL,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = [];

    $presets = get_imagecrop_styles();
    // Make sure people don't select current preset.
    if ($key = array_search(arg(5), $presets)) {
      unset($presets[$key]);
    }

    if (count($presets) > 0) {
      $form['image_style'] = [
        '#title' => t('Use the crop settings from'),
        '#type' => 'select',
        '#options' => $presets,
        '#default_value' => $this->configuration['image_style'],
      ];
    }
    else {
      $form['imagecrop_warning'] = [
        '#value' => t('No image style is found with the javascript_crop action so far. If you want to take advantage of this module, you will need to create at least one image style with that action.'),
      ];
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);
    $this->configuration['image_style'] = $form_state->getValue('image_style');
  }
}
