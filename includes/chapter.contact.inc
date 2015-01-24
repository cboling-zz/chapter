<?php
// $Id: $
/**
 * @file
 *
 * Contains forms for contact add/modify.   This is a tabbed-multistep form
 * with the design goal of simplifying contact management
 */
/**
 * Include the utility module
 */
include_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'chapter') . '/includes/chapter.util.inc');

/**
 * Get a list of user IDs that are available for associating with a contact
 *
 * This method is used to return a list of available (non-taken) user IDs
 * that can be associated with a contact
 *
 * @param $form_state
 * @param node         $node       If non-null, an existing contact node is being
 *                                 edited.
 */
function chapter_contact_user_ids(&$form_state, $node) {

  // Add 'blank' entry at the top so we can remove a user-id from the contact's data
  // should the user request it.
  // Initially, NID=0 for none-selected

  $users    = array();
  $users[0] = 'None';
  $select   = 0;

  // If someone alread selected, place it next

  if ($node != NULL) {
    $lang  = $node->language;
    $delta = 0;                // TODO: What is the proper way here...

    $uid = $node->chapter_user_account[$lang][$delta]['uid'];

    if ($uid != NULL) {
      $user           = user_load($uid);
      $select         = $user->uid;
      $users[$select] = check_plain($user->name) . ' (' . check_plain($user->mail) . ')';
    }
  }
  // Query DB for list of all user id's that are not in use by contacts

  $args  = array(':value' => 'NULL');
  $query = 'SELECT u.uid, u.name, u.mail ' .
            'FROM {users} u ' .
            'WHERE u.uid NOT IN (' .
  				'SELECT n.chapter_user_account_uid ' .
            	'FROM {field_data_chapter_user_account} n ' .
            	'WHERE n.chapter_user_account_uid != :value)';

  $result = db_query($query, $args);

  foreach ($result as $record) {
    // Skip the anonymous (0) UID and any selected already

    if (($record->uid != 0) && ($record->uid != $select)) {
      $users[$record->uid] = check_plain($record->name) .
      						 ' (' . check_plain($record->mail) . ')';
    }
  }
  return array('select' => $select,
               'list'   => $users);
}

/**
 * First page/tab of the contact form
 *
 * 'Name Information' Handles these fields
 *
 * Title, First, Middle, Last, Suffix
 * Nickname
 * Phone Numbers - Home, work, cell, fax
 * Account link
 * Notes
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 * @param unknown_type $contact_id
 */
function chapter_contact_form($form, &$form_state) {

  // Construct our title dynamically and set deaults accordingly.  First
  // get the type of contact being created and detect if we just now
  // changed types.

 // $contact_id = !empty($form_state['build_info']['args']) ?
 //                $form_state['build_info']['args'][0] : NULL;

  $type = !empty($form_state['values']['contact_type']) ?
                 $form_state['values']['contact_type'] : 'New';

  $last_type = !empty($form_state['chapter']['last_type']) ?
              $form_state['chapter']['last_type'] : 'New';

  $form_state['chapter']['last_type'] = $type;

  $contact = !empty($form_state['values']['contact']) ?
              $form_state['values']['contact'] : '';

  $last_contact = !empty($form_state['chapter']['last_contact']) ?
                  $form_state['chapter']['last_contact'] : '';

  $form_state['chapter']['last_contact'] = $contact;

  // Need to reset any pre-calculated values

  if (($type != $last_type) || ($contact != $last_contact)) {
    $form_state['chapter']['uid-list']   = array();
    $form_state['chapter']['uid-select'] = NULL;
  }
  // Do we already chapter fields and use some of that information to manage the for

  if (empty($form_state['chapter']['fields'])) {
    // No, generate it

    $form_state['chapter']['fields'] = _chapter_installed_fields();
  }
  // Set default values

  $title     = '';
  $first     = '';
  $middle    = '';
  $last      = '';
  $suffix    = '';
  $nickname  = '';
  $account   = '';
  $homephone = '';
  $workphone = '';
  $cellphone = '';
  $faxphone  = '';
  $node      = NULL;

  if ($type == 'Existing') {
    if (!empty($form_state['values']['contact'])) {

      // TODO: Need to disallow a colon ':' in the title or user names so our explodes work as expected.
      // Strip of trailing node ID and look up actual node

      $parts = explode(':', $form_state['values']['contact']);

      if (count($parts) >= 2) {
        $nid = $parts[count($parts) - 1];

        $node = node_load($nid);

        if (!empty($node)) {
          $lang  = $node->language;
          $delta = 0;                // TODO: What is the proper way here...

          $title     = !empty($node->chapter_contact_title[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_title[$lang][$delta]['safe_value'] : '';
          $first     = !empty($node->chapter_contact_first_name[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_first_name[$lang][$delta]['safe_value'] : '';
          $middle    = !empty($node->chapter_contact_middle_name[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_middle_name[$lang][$delta]['safe_value'] : '';
          $last      = !empty($node->chapter_contact_last_name[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_last_name[$lang][$delta]['safe_value'] : '';
          $suffix    = !empty($node->chapter_contact_suffix[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_last_name[$lang][$delta]['safe_value'] : '';
          $nickname  = !empty($node->chapter_contact_nickname[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_nickname[$lang][$delta]['safe_value'] : '';
          $account   = !empty($node->chapter_contact_user_account[$lang][$delta]['safe_value']) ?
                       $node->chapter_contact_user_account[$lang][$delta]['safe_value'] : '';
          $homephone = !empty($node->chapter_home_number[$lang][$delta]['safe_value']) ?
                       $node->chapter_home_number[$lang][$delta]['safe_value'] : '';
          $workphone = !empty($node->chapter_work_number[$lang][$delta]['safe_value']) ?
                       $node->chapter_work_number[$lang][$delta]['safe_value'] : '';
          $cellphone = !empty($node->chapter_cell_number[$lang][$delta]['safe_value']) ?
                       $node->chapter_cell_number[$lang][$delta]['safe_value'] : '';
          $faxphone  = !empty($node->chapter_fax_number[$lang][$delta]['safe_value']) ?
                       $node->chapter_fax_number[$lang][$delta]['safe_value'] : '';
        }
      }
    }
  }
  $full_name = !empty($nickname) ? $nickname : $first;

  if (!empty($middle) && empty($nickname)) {
    $full_name = $full_name . ' ' . $middle;
  }
  if (!empty($last)) {
    $full_name = $full_name . ' ' . $last;
  }
  // Provide a radio button that can be either 'new' or 'existing'

  $form['contact_type'] = array(
    '#type'          => 'radios',
    '#options'       => drupal_map_assoc(array(t('New'), t('Existing'))),
    '#default_value' => $type,
    '#title'         => t('Type of contact?'),
    '#ajax'          => array(
      'callback' => 'chapter_existing_contact_callback',
      'wrapper'  => 'chapter-contact-info-div',
    //  'method'   => 'replace',
      //'effect'   => 'fade',
      //'speed'    => 'fast',
      'progress' => array('type' => 'none'),
    ),
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
  // Do we already have a list of all available contacts.  This is
  // saved in the form state so the query is only ran once per page
  // displa

  if (empty($form_state['chapter']['contact-list'])) {
    // No, generate one

    $form_state['chapter']['contact-list'] = chapter_get_contact_names();
  }
  // TODO: Below, the query always runs when the page is loaded.
  //       Figure out how to do this only when 'existing' is selected
  $form['contact_existing']['contact'] = array(
    '#type'        => 'select',
    '#title'       => t('Select a Contact:'),
    '#description' => t('Select an existing contact to modify.'),
    '#options'     => $form_state['chapter']['contact-list'],
    '#ajax' => array(
      'callback' => 'chapter_existing_contact_callback',
      'wrapper'  => 'chapter-contact-info-div',
  //    'method'   => 'replace',
      //'effect'   => 'fade',
      //'speed'    => 'fast',
      'progress' => array('type' => 'none'),
    ),
  );
  //////////////////////////////////////////////////////
  // 1st data entry area  (Primary Information)

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
  $form['contact_info']['full_name'] = array(
   '#type'  => 'item',
   '#title' => check_plain($full_name),
  );
  $form['contact_info']['title'] = array(
    '#type'      => 'textfield',
    '#title'     => empty($form_state['chapter']['fields']['chapter_contact_title']['label']) ?
                    '' : $form_state['chapter']['fields']['chapter_contact_title']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_contact_title']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_contact_title']['settings']['max_length'],
    '#value'     => check_plain($title),
  );
  $form['contact_info']['first'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_contact_first_name']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_contact_first_name']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_contact_first_name']['settings']['max_length'],
    '#value'     => check_plain($first),
  );
  $form['contact_info']['middle'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_contact_middle_name']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_contact_middle_name']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_contact_middle_name']['settings']['max_length'],
    '#value'     => check_plain($middle),
  );
  $form['contact_info']['last'] = array(
    '#type' 	 => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_contact_last_name']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_contact_last_name']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_contact_last_name']['settings']['max_length'],
    '#required'  => TRUE,
    '#value'     => check_plain($last),
  );
  $form['contact_info']['nickname'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_contact_nickname']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_contact_nickname']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_contact_nickname']['settings']['max_length'],
    '#value'     => check_plain($nickname),
  );
  //////////////////////////////////////////////////////////////
  // 2nd data entry area (Phone Numbers - Home, work, cell, fax)
  //////////////////////////////////////////////////////
  // 1st data entry area  (Primary Information)

  $form['contact_info']['contact_numbers'] = array(
    '#title' => t("Phone Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  $form['contact_info']['contact_numbers']['homephone'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_home_number']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_home_number']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_home_number']['settings']['max_length'],
    '#value'     => check_plain($homephone),
  );
  $form['contact_info']['contact_numbers']['workphone'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_work_number']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_work_number']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_work_number']['settings']['max_length'],
    '#value'     => check_plain($workphone),
  );
  $form['contact_info']['contact_numbers']['cellphone'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_cell_number']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_cell_number']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_cell_number']['settings']['max_length'],
    '#value'     => check_plain($cellphone),
  );
  $form['contact_info']['contact_numbers']['faxphone'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_fax_number']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_fax_number']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_fax_number']['settings']['max_length'],
    '#value'     => check_plain($faxphone),
  );
  //////////////////////////////////////////////////////////////
  // 3rd data entry area (Drupal Account / User ID)
  ///////////////////////////////////////////////////////////////
  // Drupal Account / User ID

  $form['contact_info']['contact_account'] = array(
    '#title' => t("Account Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  // Do we already have a list of all possible User IDs/Names.  This is
  // saved in the form state so the query is only ran once per page
  // displa

  if (empty($form_state['chapter']['uid-list'])) {
    // No, generate one

    $uid_array = chapter_contact_user_ids($form_state, $node);

    $form_state['chapter']['uid-select'] = $uid_array['select'];
    $form_state['chapter']['uid-list']   = $uid_array['list'];
  }
  $form['contact_info']['contact_account']['userid'] = array(
    '#type'        => 'select',
    '#title'       => t('User Account:'),
    '#description' => t('Select an user account to associate with this contact, if any'),
    '#options'     => $form_state['chapter']['uid-list'],
    '#value'       => $form_state['chapter']['uid-select'],
  );
  /* TODO
   * Primary & Secondary Physical Address
   * Primary & Secondary Electronic Address
   * Associated Chapter
   * Notes
   */
  // Adds a simple submit button that refreshes the form and clears its contents -- this is the default behavior for forms.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ($type == 'New') ? 'Submit' : 'Apply',
//    '#states' => array(
  //    'disabled' => array('modified' => array('value' => 0),),
 //   ),
  );
  return $form;
}

/**
 * AJAX Callback for change of existing user
 *
 * @return renderable array (the checkboxes fieldset)
 */
function chapter_existing_contact_callback($form, $form_state) {
  return $form['contact_info'];
}

/**
 * Validate contact menu input
 *
 * This function is called to validate user input to the 'chapter_contact_form'
 * settings menu.
 *
 * @param $form
 * @param $form_state
 */
function chapter_contact_form_validate($form, &$form_state) {
  watchdog('chapter', 'entered chapter_contact_form_validate');

 // $db_integrity  = $form_state['values']['chapter_db_integrity'];
  $evaluate_this = TRUE;

  if (!$evaluate_this) {
    form_set_error('existing', t('You made a boo boo.'));
  }
}

/**
 * Apply changes
 *
 * Handles the submission of new information
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function chapter_contact_form_submit($form, &$form_state) {

  watchdog('chapter', 'entered chapter_contact_form_submit');

  drupal_set_message(t('Submitting values: @values',
                array('@values' => var_export($form_state['values'], TRUE))));

}

/**
 * Address page/tab of the contact entry form
 *
 * 'Physical and email' entries
 *
 * Primary & Secondary Physical Address
 * Primary & Secondary Electronic Address
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function chapter_address_form($form, &$form_state) {

  // Do we already chapter fields and use some of that information to manage the for

  if (empty($form_state['chapter']['fields'])) {
    // No, generate it

    $form_state['chapter']['fields'] = _chapter_installed_fields();
  }
  // Set default values

  $primary_description = "";
  $primary_premise     = "";
  $primary_subpremise  = "";
  $primary_locality    = "";
  $primary_state       = "";
  $primary_postalcode  = "";
  $primary_country     = "";
  $primary_directions  = "";
  $primary_notes       = "";

  // Load up types in use

  $loc_1_type = !empty($form_state['values']['loc_1_type']) ?
                 $form_state['values']['loc_1_type'] : t('n/a');
  $loc_2_type = !empty($form_state['values']['loc_2_type']) ?
                 $form_state['values']['loc_2_type'] : t('n/a');
  $elec_1_type = !empty($form_state['values']['elec_1_type']) ?
                  $form_state['values']['elec_1_type'] : t('n/a');
  $elec_2_type = !empty($form_state['values']['elec_2_type']) ?
                  $form_state['values']['elec_2_type'] : t('n/a');

  //////////////////////////////////////////////////////
  // 1st data entry area  (Primary Information)

  $form['primary_location'] = array(
    '#title' => t("Primary Physical Address Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] below.
    '#prefix'      => '<div id="chapter-primary-location-div">',
    '#suffix'      => '</div>',
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  // Provide a radio button that can be either 'n/a', 'new' or 'existing'
  $form['primary_location']['loc_1_type'] = array(
    '#type'    => 'radios',
    '#options' => drupal_map_assoc(array(t('n/a'), t('New'), t('Existing'))),
    '#title'   => t('Primary Physical Location?'),
    '#value'   => $loc_1_type,
    '#default_value' => $loc_1_type,
    '#ajax'    => array(
      'callback' => 'chapter_existing_loc_1_callback',
      'wrapper'  => 'chapter-primary-location-div',
    //  'method'   => 'replace',
      //'effect'   => 'fade',
      //'speed'    => 'fast',
      'progress' => array('type' => 'none'),
    ),
  );
  // An Existing Contact
  $form['primary_location']['primary_existing'] = array(
    '#title'    => t('Existing Location Information'),
    '#type'     => 'select',
    '#options'  => drupal_map_assoc(array(t('aaa'), t('bbb'), t('ccc'))),
    '#states'   => array(
      'visible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('Existing')),
      ),
    ),
  );
  // Description
  $form['primary_location']['primary_description'] = array(
    '#type'      => 'textfield',
    '#title'     => 'Brief Description:',
    '#size'      => 64,
    '#maxlength' => 256,
    '#value'     => check_plain($primary_description),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // Address 1
  $form['primary_location']['primary_premise'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_premise']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_premise']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_premise']['settings']['max_length'],
    '#value'     => check_plain($primary_premise),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // Address 2
  $form['primary_location']['primary_subpremise'] = array(
    '#type'      => 'textfield',
    '#title'     => !empty($form_state['chapter']['fields']['chapter_subpremise']['label']) ?
                    $form_state['chapter']['fields']['chapter_subpremise']['label'] . ':' : '',
    '#size'      => $form_state['chapter']['fields']['chapter_subpremise']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_subpremise']['settings']['max_length'],
    '#value'     => check_plain($primary_subpremise),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // City
  $form['primary_location']['primary_locality'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_locality']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_locality']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_locality']['settings']['max_length'],
    '#value'     => check_plain($primary_locality),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // State
  $form['primary_location']['primary_state'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_state']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_state']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_state']['settings']['max_length'],
    '#value'     => check_plain($primary_state),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // Postal
  $form['primary_location']['primary_postalcode'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_postalcode']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_postalcode']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_postalcode']['settings']['max_length'],
    '#value'     => check_plain($primary_postalcode),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // Country
  $form['primary_location']['primary_country'] = array(
    '#type'      => 'textfield',
    '#title'     => $form_state['chapter']['fields']['chapter_country']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_country']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_country']['settings']['max_length'],
    '#value'     => check_plain($primary_country),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // Directions
  // TODO: How do we enable the WYSISYG controls

  $form['primary_location']['primary_directions'] = array(
    '#type'      => 'text_format',
    '#format'    => 'full_html',
    '#title'     => $form_state['chapter']['fields']['chapter_directions']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_directions']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_directions']['settings']['max_length'],
    '#value'     => check_plain($primary_directions),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );
  // Notes
  // TODO: How do we handle the summary area.
  // TODO: How do we enable the WYSISYG controls
  $form['primary_location']['primary_notes'] = array(
    '#type'      => 'text_format',
    '#format'    => 'full_html',
    '#title'     => $form_state['chapter']['fields']['chapter_directions']['label'] . ':',
    '#size'      => $form_state['chapter']['fields']['chapter_directions']['settings']['display_size'],
    '#maxlength' => $form_state['chapter']['fields']['chapter_directions']['settings']['max_length'],
    '#value'     => check_plain($primary_notes),
    '#states'    => array(
      'invisible' => array(
        ':input[name="loc_1_type"]' => array('value' => t('n/a')),
      ),
    ),
  );

  ////////////////////////////////////////////////////////////////////

  $form['secondary_location'] = array(
    '#title' => t("Secondary Physical Address Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] below.
    '#prefix'      => '<div id="chapter-secondary-location-div">',
    '#suffix'      => '</div>',
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  $form['secondary_location']['loc_2_type'] = array(
    '#type'    => 'radios',
    '#options' => drupal_map_assoc(array(t('n/a'), t('New'), t('Existing'))),
    '#title'   => t('Secondary Physical Location?'),
    '#default_value' => $loc_2_type,
    '#value'   => $loc_2_type,
    '#ajax'    => array(
      'callback' => 'chapter_existing_loc_2_callback',
      'wrapper'  => 'chapter-secondary-location-div',
    //  'method'   => 'replace',
      //'effect'   => 'fade',
      //'speed'    => 'fast',
      'progress' => array('type' => 'none'),
    ),
  );
  // Description
  // Address 1
  // Address 2
  // City
  // State
  // Postal
  // Country
  // Directions
  // Notes



  $form['primary_electronic'] = array(
    '#title' => t("Primary Electronic Address Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] below.
    '#prefix'      => '<div id="chapter-primary-electronic-div">',
    '#suffix'      => '</div>',
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  $form['primary_electronic']['elec_1_type'] = array(
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array(t('n/a'), t('New'), t('Existing'))),
    '#title'   => t('Primary Electronic Contact Information?'),
    '#value'   => $elec_1_type,
    '#default_value' => $elec_1_type,
    '#ajax'    => array(
      'callback' => 'chapter_existing_elec_1_callback',
      'wrapper'  => 'chapter-primary-electronic-div',
    //  'method'   => 'replace',
      //'effect'   => 'fade',
      //'speed'    => 'fast',
      'progress' => array('type' => 'none'),
    ),
  );
  // Title
  // Email - Title
  // Email - URL
  // WWW   - Title
  // WWW   - URL
  // SMS
  // Notes



  $form['secondary_electronic'] = array(
    '#title' => t("Secondary Electronic Address Information"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] below.
    '#prefix'      => '<div id="chapter-secondary-electronic-div">',
    '#suffix'      => '</div>',
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  //'#description' => t('This is where we get automatically generated checkboxes'),
  );
  $form['secondary_electronic']['elec_2_type'] = array(
    '#type'    => 'radios',
    '#options' => drupal_map_assoc(array(t('n/a'), t('New'), t('Existing'))),
    '#title'   => t('Secondary Electronic Contact Information?'),
    '#value'   => $elec_2_type,
    '#default_value' => $elec_2_type,
    '#ajax'    => array(
      'callback' => 'chapter_existing_elec_2_callback',
      'wrapper'  => 'chapter-secondary-electronic-div',
    //  'method'   => 'replace',
      //'effect'   => 'fade',
      //'speed'    => 'fast',
      'progress' => array('type' => 'none'),
    ),
  );
  // Title
  // Email - Title
  // Email - URL
  // WWW   - Title
  // WWW   - URL
  // SMS
  // Notes



  return $form;
}

/**
 * AJAX Callback for change of primary location select
 *
 * @return renderable array (the checkboxes fieldset)
 */
function chapter_existing_loc_1_callback($form, $form_state) {
  return $form['primary_location_type'];
}

/**
 * AJAX Callback for change of secondary location select
 *
 * @return renderable array (the checkboxes fieldset)
 */
function chapter_existing_loc_2_callback($form, $form_state) {
  return $form['secondary_location_type'];
}

/**
 * AJAX Callback for change of primary electronic selection
 *
 * @return renderable array (the checkboxes fieldset)
 */
function chapter_existing_elec_1_callback($form, $form_state) {
  return $form['primary_elec_type'];
}

/**
 * AJAX Callback for change of secondary electronic address selection
 *
 * @return renderable array (the checkboxes fieldset)
 */
function chapter_existing_elec_2_callback($form, $form_state) {
  return $form['secondary_elec_type'];
}

/**
 * Validate address menu input
 *
 * This function is called to validate user input to the 'chapter_contact_form'
 * settings menu.
 *
 * @param $form
 * @param $form_state
 */
function chapter_address_form_validate($form, &$form_state) {
  watchdog('chapter', 'entered chapter_address_form_validate');

 // $db_integrity  = $form_state['values']['chapter_db_integrity'];
  $evaluate_this = TRUE;

  if (!$evaluate_this) {
    form_set_error('primary_location_type', t('You made a boo boo.'));
  }
}

/**
 * Apply address changes
 *
 * Handles the submission of new information
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function chapter_address_form_submit($form, &$form_state) {

  watchdog('chapter', 'entered chapter_address_form_submit');

  drupal_set_message(t('Submitting values: @values',
                array('@values' => var_export($form_state['values'], TRUE))));

}

/**
 * Membership page/tab of the contact entry form
 *
 * 'Chapter/membership' entries
 *
 * Associated Chapter
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function chapter_membership_form($form, &$form_state) {

  // Provide a radio button that can be either 'new' or 'existing'
  $form['membership_type'] = array(
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array(t('New'), t('Existing'))),
    '#title' => t('What type of membership?')
  );
  $form['secondary_elec_type'] = array(
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array(t('n/a'), t('New'), t('Existing'))),
    '#title' => t('Secondary Electronic Contact Information?')
  );
  return $form;
}

/**
 * Validate membership menu input
 *
 * This function is called to validate user input to the 'chapter_contact_form'
 * settings menu.
 *
 * @param $form
 * @param $form_state
 */
function chapter_membership_form_validate($form, &$form_state) {
  watchdog('chapter', 'entered chapter_membership_form_validate');

 // $db_integrity  = $form_state['values']['chapter_db_integrity'];
  $evaluate_this = TRUE;

  if (!$evaluate_this) {
    form_set_error('chapter_db_integrity', t('You made a boo boo.'));
  }
}

/**
 * Apply membership changes
 *
 * Handles the submission of new information
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function chapter_membership_form_submit($form, &$form_state) {

  watchdog('chapter', 'entered chapter_membership_form_submit');

  drupal_set_message(t('Submitting values: @values',
                array('@values' => var_export($form_state['values'], TRUE))));

}

