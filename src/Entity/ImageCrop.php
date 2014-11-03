<?php

/**
 * @file
 * Contains \Drupal\imagecrop\Entity\ImageCrop.
 */

namespace Drupal\imagecrop\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\imagecrop\ImageCropInterface;

/**
 * Defines the imagecrop entity class.
 *
 * @ContentEntityType(
 *   id = "imagecrop",
 *   label = @Translation("Image crop"),
 *   handlers = {
 *     "storage" = "Drupal\imagecrop\ImageCropStorage",
 *     "view_builder" = "Drupal\Core\Entity\EntityViewBuilder",
 *     "access" = "Drupal\Core\Entity\EntityAccessControlHandler",
 *     "form" = {
 *       "default" = "Drupal\Core\Entity\ContentEntityForm",
 *       "delete" = "Drupal\Core\Entity\ContentEntityConfirmFormBase",
 *       "edit" = "Drupal\Core\Entity\ContentEntityForm"
 *     },
 *     "translation" = "Drupal\content_translation\ContentTranslationHandler"
 *   },
 *   base_table = "imagecrop",
 *   data_table = "imagecrop_field_data",
 *   revision_table = "imagecrop_revision",
 *   revision_data_table = "imagecrop_field_revision",
 *   fieldable = TRUE,
 *   translatable = FALSE,
 *   render_cache = FALSE,
 *   entity_keys = {
 *     "id" = "icid",
 *     "revision" = "vid",
 *     "uuid" = "uuid"
 *   },
 *   permission_granularity = "entity_type",
 *   admin_permission = "administer imagecrop",
 *   links = {
 *     "canonical" = "",
 *     "delete-form" = "",
 *     "edit-form" = "",
 *     "admin-form" = ""
 *   }
 * )
 */
class ImageCrop extends ContentEntityBase implements ImageCropInterface {

  /**
   * {@inheritdoc}
   */
  public function file() {
    /** @var \Drupal\file\FileStorageInterface $storage */
    $storage = \Drupal::service('entity_manager')->getStorage('file');
    return $storage->load($this->file->target_id);
  }

  /**
   * {@inheritdoc}
   */
  public function imageStyle() {
    /** @var \Drupal\Core\Config\Entity\ConfigEntityStorageInterface $storage */
    $storage = \Drupal::service('entity_manager')->getStorage('image_style');
    return $storage->load($this->image_style->target_id);
  }

  /**
   * {@inheritdoc}
   */
  public function preSave(EntityStorageInterface $storage) {
    parent::preSave($storage);

    // If no revision author has been set explicitly, make the media owner the
    // revision author.
    if (!$this->get('revision_uid')->entity) {
      $this->set('revision_uid', \Drupal::currentUser()->id());
    }
  }

  /**
   * {@inheritdoc}
   */
  public function preSaveRevision(EntityStorageInterface $storage, \stdClass $record) {
    parent::preSaveRevision($storage, $record);

    if (!$this->isNewRevision() && isset($this->original) && (!isset($record->revision_log) || $record->revision_log === '')) {
      // If we are updating an existing node without adding a new revision, we
      // need to make sure $entity->revision_log is reset whenever it is empty.
      // Therefore, this code allows us to avoid clobbering an existing log
      // entry with an empty one.
      $record->revision_log = $this->original->revision_log->value;
    }
  }

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields['icid'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Image crop ID'))
      ->setDescription(t('The image crop ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['uuid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The image crop UUID.'))
      ->setReadOnly(TRUE);

    $fields['vid'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Revision ID'))
      ->setDescription(t('The image crop revision ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['file'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Bundle'))
      ->setDescription(t('The file that crop relates to.'))
      ->setSetting('target_type', 'file')
      ->setReadOnly(TRUE);

    $fields['image_style'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Image style'))
      ->setDescription(t('The image style that crop relates to.'))
      ->setSetting('target_type', 'image_style')
      ->setReadOnly(TRUE);

    $fields['revision_timestamp'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Revision timestamp'))
      ->setDescription(t('The time that the current revision was created.'))
      ->setQueryable(FALSE)
      ->setRevisionable(TRUE);

    $fields['revision_uid'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Revision author ID'))
      ->setDescription(t('The user ID of the author of the current revision.'))
      ->setSetting('target_type', 'user')
      ->setQueryable(FALSE)
      ->setRevisionable(TRUE);

    $fields['revision_log'] = BaseFieldDefinition::create('string_long')
      ->setLabel(t('Revision Log'))
      ->setDescription(t('The log entry explaining the changes in this revision.'))
      ->setRevisionable(TRUE)
      ->setTranslatable(TRUE);

    return $fields;
  }

}
