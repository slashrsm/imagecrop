<?php

/**
 * @file
 * Contains \Drupal\imagecrop\ImageCropStorageInterface.
 */

namespace Drupal\imagecrop;
use Drupal\Core\Entity\EntityBundleListenerInterface;
use Drupal\Core\Entity\Schema\DynamicallyFieldableEntityStorageSchemaInterface;
use Drupal\Core\Entity\Sql\SqlEntityStorageInterface;

/**
 * Provides an interface defining an image crop storage controller.
 */
interface ImageCropStorageInterface extends SqlEntityStorageInterface, DynamicallyFieldableEntityStorageSchemaInterface, EntityBundleListenerInterface {

}
