<?php
// $Id: $
/**
 * @file
 *
 * Contains forms introduction for the Chapter module
 */
/**
 * Include the utility module
 */
include_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'chapter') . '/includes/chapter.util.inc');

/**
 * Tools introduction form
 *
 */
function chapter_intro_form($form, &$form_state) {
  // Determine our rights

  $add_contact = TRUE;
  $edit_contact = TRUE;
  $img_path = drupal_get_path('module', 'chapter') . '/images/';

  // Provide a radio button that can be either 'new' or 'existing'

  $form['intro'] = array(
    '#prefix' => '<div class="chapter-tools-intro-main">',
    '#markup' => t('The Non-Profit Chapter module provides a variety of tools that ' .
                  'can be used to manage the day-to-day communications tasks of a ' .
                  'small non-profit organization.<br ><br >' .
                  'Based on your account\'s access rights and roles, the following ' .
                  'tools are available:<br ><br >'),
    '#suffix' => '</div>',
  );
  $form['contact'] = array(
    '#title' => t("Contacts / Members"),
    '#description' => t('Contact edit and creation forms'),
    '#prefix'      => '<div id="chapter-tools-intro-contact">',
    '#suffix'      => '</div>',
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#access'      => $add_contact || $edit_contact,
  );
  $form['contact']['new'] = array(
    '#type'   => 'image_button',
    '#src'    => $img_path . 'new_contact.png',
    '#value'  => 'Add New Contact',
    '#name'   => 'new_contact',
    '#title'  => t('Add new contact'),
    '#access' => $add_contact,
  );
  $form['contact']['edit'] = array(
    '#type'   => 'image_button',
    '#src'    => $img_path . 'edit_contact.png',
    '#value'  => 'Edit Existing Contact',
    '#name'   => 'new_contact',
    '#title'  => t('Edit existing contact'),
    '#access' => $edit_contact,
  );
  // An Existing Contact
  $form['contact_existing'] = array(
    '#type'     => 'fieldset',
    '#title'    => t('Existing Contact Information'),
    '#states'   => array(
      'visible' => array(
        ':input[name="contact_type"]' => array('value' => t('Existing')),
      ),
    ),
  );
  //////////////////////////////////////////////////////
  // Existing contact list

  $form['contact_info'] = array(
    '#title' => t("Primary Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#prefix'      => '<div id="chapter-contact-info-div">',
    '#suffix'      => '</div>',
    '#type'        => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  return $form;
}

/**
 * Apply changes
 *
 * Handles the submission of new information
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function chapter_intro_form_submit($form, &$form_state) {

  watchdog('chapter', 'entered chapter_intro_form_submit');

  // TODO: Find out how to redirect to actual 'new' or 'existing' form

  switch ($form_state['clicked_button']['#name']) {
    case 'new_contact':
      $form_state['redirect'] = 'chapter/forms/contact';
      break;

    case 'edit_contact':
      $form_state['redirect'] = 'chapter/forms/contact'';
      break;
  }
}
