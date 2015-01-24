<?php
// $Id: $
/**
 * @file
 *
 * Contains administration of the chapter core modules
 */

/**
 * Form builder. Provides an administration form or the module
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function chapter_admin() {
  //menu_rebuild();

  $form = array();
  $vars = _chapter_installed_variables();

  /////////////////////////////////////////////////
  // Database options

  $form['database'] = array(
    '#type'        => 'fieldset',
    '#title'	   => t('Database Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  	'#tree'        => TRUE,
    '#description' => t("Options to control the behaviour of some database operations."),
  );
  $form['database']['chapter_db_integrity'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Database Integrity'),
    '#default_value' => variable_get('chapter_db_integrity', $vars['chapter_db_integrity']['default']),
    '#description'   => t("This should be set to true if you do not want deletes of nodes that " .
                          "are referenced by other nodes to be deleted.  The actual behavior is " .
                          "controlled by the 'foreign key' option below if true.  If set to false, " .
  					      "deletes can be performed at any time and the existing references to the " .
                          "newly delted know will be left as-is (dangling...)"),
    '#required'      => FALSE,
  );
  $form['database']['chapter_db_fk_options'] = array(
    '#type'          => 'radios',
    '#title'         => t('Foreign key operations:'),
    '#options'       => drupal_map_assoc(array(
    						CHAPTER_FK_DELETE_RESTRICT => t('restrict'),
  							CHAPTER_FK_DELETE_NULLIFY  => t('nullify'),
                            CHAPTER_FK_DELETE_IGNORE   => t('ignore'))),
    '#default_value' => variable_get('chapter_db_fk_options', $vars['chapter_db_fk_options']['default']),
    '#description'   => t("When a referenced node is deleted, nodes that reference " .
                          "them may operate on their reference based on this options. " .
                          "The allowed values are:<br />" .
                          "<ul><li>Restrict - You cannot delete the referenced node until " .
                          "all other nodes referencing it is deleted or the key set to NULL</li>" .
                          "<li>Nullify - &nbsp;Automatically set key in the nodes referencing " .
                          "the deletednode to NULL. The delete is allowed.</li>" .
                          "<li>Ignore - &nbsp;&nbsp;Do not update the key in the referencing " .
                          "node and allow the delete.</li></ul>"),
    '#required'      => FALSE,
    '#states'		 => array(
      'visible' => array(':input[name="chapter_db_integrity"]' => array('checked' => TRUE))),
  );
  /////////////////////////////////////////////////
  // Dues options

  $form['dues'] = array(
    '#type'        => 'fieldset',
    '#title'	   => t('Local Chapter Dues'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  	'#tree'        => TRUE,
    '#description' => t("Options related to local membership dues."),
  );
  $form['dues']['chapter_local_dues'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Local dues required.'),
    '#default_value' => variable_get('chapter_local_dues', $vars['chapter_local_dues']['default']),
    '#description'   => t("If 'local_dues' is selected, then members must pay the indicated " .
                          "amount to be considered an 'active' member of the local chapter. " .
  					      "National organization dues (if any) are not supported by this module."),
    '#required'      => FALSE,
  );
  $form['dues']['chapter_local_dues_amount'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Amount'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_local_dues_amount', $vars['chapter_local_dues_amount']['default']),
    '#description'   => "Please enter the yearly dues amount.  Leave blank or enter zero " .
                        "yearly dues are required for the local chapter.",
    '#size'          => 7,
    '#maxlength'     => 7,
    '#element_validate' => array('_chapter_validate_integer_positive_or_blank'),
  );
  $form['dues']['chapter_local_dues_currency'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Currency name'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_local_dues_currency', $vars['chapter_local_dues_currency']['default']),
    '#description'   => "Enter the name of your currency.",
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['dues']['chapter_local_dues_symbol'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Currency symbol'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_local_dues_symbol', $vars['chapter_local_dues_symbol']['default']),
    '#description'   => "Enter the valid HTML symbol(s) for your currency.  For example, &euro; is " .
                        "specified with the string \&euro;",
    '#size'          => 16,
    '#maxlength'     => 16,
  );
  /////////////////////////////////////////////////
  // Guests options
  //
  // TODO Move this to event module

  $form['guests'] = array(
    '#type'        => 'fieldset',
    '#title'	   => t('Event Attendance Policies'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  	'#tree'        => TRUE,
  );
  $form['guests']['chapter_guest_event_limit'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Maximum Guest Attendance Threshold'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_guest_event_limit', $vars['chapter_guest_event_limit']['default']),
    '#description'   => "Enter the maximum number of times a person may be invited as a " .
                        "guest before they must join.  This is required by some chapter's " .
                        "liability insurance. Enter zero or leave blank if there is not a " .
                        "guest attendane limitation.",
    '#size'          => 4,
    '#maxlength'     => 4,
    '#element_validate' => array('_chapter_validate_integer_positive_or_blank'),
  );
  $form['guests']['chapter_active_limit'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Active Member Threshold'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_active_limit', $vars['chapter_active_limit']['default']),
    '#description'   => "The active limit is the maximum number of days that is allowed to " .
                        "go by before declaring a member as 'inactive'.  Besides possibly " .
                        "needing to pay national or local dues, the 'active' limit can be " .
                        "used to automatically age off people who may no longer belong.  " .
                        "Active members are a special role and may be assigned more " .
                        "permissions that just an authenticated user.  Set to zero or " .
                        "leave blank to disable this feature.",
    '#size'          => 5,
    '#maxlength'     => 5,
    '#element_validate' => array('_chapter_validate_integer_positive_or_blank'),
  );
  /////////////////////////////////////////////////
  // Roles options

  $form['roles'] = array(
    '#type'        => 'fieldset',
    '#title'	   => t('Roles Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  	'#tree'        => TRUE,
    '#description' => "The following roles are automatically created at installation and " .
                      "are used to describe various board and other positions that are " .
                      "typically held for one year or more.  If a position is not used, " .
                      "leave the field blank.<br \>Once a posistion is used, please do not " .
                      "try an use if for a different permission if possible, chose a " .
                      "previously empty position instead.<br \>" .
                      "Roles may be assigned to multiple user accounts.  For instance, if " .
                      "your organization has both a chairperson and co-chair, you may just " .
                      "want to assign both of them to the 'Chairperson' position as the " .
                      "co-chair may often have to fill in for the chairperson when they are " .
                      "not available.",
  );
  $form['roles']['chapter_chair'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Chairperson'),
    '#required'      => TRUE,
    '#default_value' => variable_get('chapter_chair', $vars['chapter_chair']['default']),
    '#description'   => "The title for the chairperson or director for the chapter.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_treasurer'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Treasurer'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_treasurer', $vars['chapter_treasurer']['default']),
    '#description'   => "The title for the treasurer of the chapter.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_editor'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Editor'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_editor', $vars['chapter_editor']['default']),
    '#description'   => "The title for the position that who's primary responsibility is the " .
                        "newsletter editor or is responsible for notifying the chapter of " .
                        "any upcoming events or changes in the organization,",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_secretary'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Secretary'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_secretary', $vars['chapter_secretary']['default']),
    '#description'   => "The title for the position that is responsible for chapter " .
                        "correspondence." .
                        "" .
                        "",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_education'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Education'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_education', $vars['chapter_education']['default']),
    '#description'   => "The title for the position that is responsible for general education " .
                        "or for maintaining the history of the chapter.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_webmaster'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Webmaster'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_webmaster', $vars['chapter_webmaster']['default']),
    '#description'   => "The title of the person who is responsible for maintaining the " .
                        "chapter's website.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_programs'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Programs'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_programs', $vars['chapter_programs']['default']),
    '#description'   => "The title for the position that is primarily in charge of " .
                        "event organization.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_board'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Board Member'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_board', $vars['chapter_board']['default']),
    '#description'   => "A general role for a board member that does not easily fit into " .
                        "a previous category.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_other_1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Other-1'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_other_1', $vars['chapter_other_1']['default']),
    '#description'   => "This field is available for general purpose use.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_other_2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Other-2'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_other_2', $vars['chapter_other_2']['default']),
    '#description'   => "This field is available for general purpose use.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_other_3'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Other-3'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_other_3', $vars['chapter_other_3']['default']),
    '#description'   => "This field is available for general purpose use.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_other_4'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Other-4'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_other_4', $vars['chapter_other_4']['default']),
    '#description'   => "This field is available for general purpose use.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_other_5'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Other-5'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_other_5', $vars['chapter_other_5']['default']),
    '#description'   => "This field is available for general purpose use.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['roles']['chapter_active_member'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Active Member'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_active_member', $vars['chapter_active_member']['default']),
    '#description'   => "To encourage event attendance (or paying of dues) an " .
                        "additional role of 'active member' is defined that can be " .
                        "provided with additional access rights to some content and " .
                        "site features. All 'active member's are 'authenticated " .
                        "user's but not all 'authenticated user' are active members " .
                        "of the chapter.",
    // TODO Put verbage in for the default permissions granted to this position.
    '#size'          => 24,
    '#maxlength'     => 24,
  );

  $form['roles']['chapter_board_pre_tenure_days'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Pre-tenure r/o days'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_board_pre_tenure_days', $vars['chapter_board_pre_tenure_days']['default']),
    '#description'   => "To assist incoming board members, the value of this field " .
                        "grants appropriate read-only access for the number of days " .
                        "before their tenure is to start. To disable, specify a value of zero.",
    '#size'          => 4,
    '#maxlength'     => 4,
  );

  $form['roles']['chapter_board_post_tenure_days'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Post-tenure r/o days'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_board_post_tenure_days', $vars['chapter_board_post_tenure_days']['default']),
    '#description'   => "To assist new board members, the value of this field grants " .
                        "appropriate read-only access for previous board members to " .
                        "assist the new board in their responsibilities.  To disable, " .
              			"specify a value of zero.",
    '#size'          => 4,
    '#maxlength'     => 4,
  );

  /////////////////////////////////////////////////
  // Uninstall debug options

  $form['uninstall'] = array(
    '#type'        => 'fieldset',
    '#title'	   => t('Uninstall Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  	'#tree'        => TRUE,
  	'#description' => t('These values can be used to retain various fields and variables ' .
                        'that are deleted upon removal of the product.  These are mostly ' .
                        'intended to be used during debugging of this module.'),
  );
  $form['uninstall']['chapter_retain_content'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Retain content'),
    '#default_value' => variable_get('chapter_retain_content', $vars['chapter_retain_content']['default']),
    '#description'   => t("Do not delete content specific to this module during uninstall."),
    '#required'      => FALSE,
  );
  $form['uninstall']['chapter_retain_content_types'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Retain content'),
    '#default_value' => variable_get('chapter_retain_content_types', $vars['chapter_retain_content_types']['default']),
    '#description'   => t("Maintain this module's content-types during uninstall."),
    '#required'      => FALSE,
  );
  $form['uninstall']['chapter_retain_roles'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Retain roles'),
    '#default_value' => variable_get('chapter_retain_content_types', $vars['chapter_retain_content_types']['default']),
    '#description'   => t("Maintain this module's auto-installed roles and permissions during uninstall."),
    '#required'      => FALSE,
  );
  $form['uninstall']['chapter_retain_variables'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Retain variables'),
    '#default_value' => variable_get('chapter_retain_content_types', $vars['chapter_retain_content_types']['default']),
    '#description'   => t("Maintain this module's site variables during uninstall."),
    '#required'      => FALSE,
  );
  /////////////////////////////////////////////////
  // Misc options

  $form['misc'] = array(
    '#type'        => 'fieldset',
    '#title'	   => t('Miscellaneous Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  	'#tree'        => TRUE,
  );
  $form['misc']['chapter_next_member_id'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Next member ID'),
    '#required'      => FALSE,
    '#default_value' => variable_get('chapter_next_member_id', $vars['chapter_next_member_id']['default']),
    '#description'   => t("Next membership_id is used to seed a table that " .
                          "automatically creates a membership id if one is " .
                          "not provided. Each get will increment the id number. " .
                          "For some installations, you may not want the ID to be " .
                          "unique so that you can assign a single ID to both " .
                          "spouse or an entire family..."),
    '#size'          => 24,
    '#maxlength'     => 24,
  );
  $form['misc']['chapter_auto_member_id'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Automatic Member ID generation'),
    '#default_value' => variable_get('chapter_auto_member_id', $vars['chapter_auto_member_id']['default']),
    '#description'   => t("If false, an administrator must manually enter " .
                          "membership IDs.  Leave it at 'FALSE' if you do not " .
                          "care about membership IDs."),
    '#required'      => FALSE,
  );
  $form['misc']['chapter_hide_add_content'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide add-content'),
    '#default_value' => variable_get('chapter_hide_add_content', $vars['chapter_hide_add_content']['default']),
    '#description'   => t("Hide the module defined content types from the 'add " .
                          "content' type menu and only the installed web forms can " .
                          "be used to add our content types.  Roles with explicit " .
                          "'administer nodes' rights always can use the 'add content' " .
                          "menu method of inserting content"),
    '#required'      => FALSE,
  );
  $form['#submit'][] = 'chapter_admin_submit';

  //$form['actions'] = array('#type' => 'actions');
  //$form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return system_settings_form($form);
}

/**
 * Commit chapter settings
 *
 * This method is called when the user click's the submit button for the non-
 * profit chapter settings form.
 *
 * @param $form
 * @param $form_state
 */
function chapter_admin_submit($form, $form_state) {
  watchdog('chapter', 'entered chapter_admin_submit');

  // Save the straight forward ones first.  They just need to update the values

  variable_set('chapter_db_integrity', $form_state['values']['database']['chapter_db_integrity']);
  variable_set('chapter_db_fk_options', $form_state['values']['database']['chapter_db_fk_options']);

  variable_set('chapter_local_dues', $form_state['values']['dues']['chapter_local_dues']);
  variable_set('chapter_local_dues_amount', $form_state['values']['dues']['chapter_local_dues_amount']);
  variable_set('chapter_local_dues_currency', check_plain($form_state['values']['dues']['chapter_local_dues_currency']));
  variable_set('chapter_local_dues_symbol', check_plain($form_state['values']['dues']['chapter_local_dues_symbol']));

  // NOTE: Changing the guest limit or inactive limit does not change the status
  //       of any contacts.  If limits are reduced, they will be picked up on the
  //       next CRON run or RSVP submission.

  variable_set('chapter_guest_event_limit', $form_state['values']['guests']['chapter_guest_event_limit']);
  variable_set('chapter_active_limit', $form_state['values']['guests']['chapter_active_limit']);

  variable_set('chapter_retain_content', $form_state['values']['uninstall']['chapter_retain_content']);
  variable_set('chapter_retain_content_types', $form_state['values']['uninstall']['chapter_retain_content_types']);
  variable_set('chapter_retain_roles', $form_state['values']['uninstall']['chapter_retain_roles']);
  variable_set('chapter_retain_variables', $form_state['values']['uninstall']['chapter_retain_variables']);

  variable_set('chapter_next_member_id', $form_state['values']['misc']['chapter_next_member_id']);
  variable_set('chapter_auto_member_id', $form_state['values']['misc']['chapter_auto_member_id']);
  variable_set('chapter_hide_add_content', $form_state['values']['misc']['chapter_hide_add_content']);
//  variable_set('chapter_use_relations', $form_state['values']['misc']['chapter_use_relations']);

  variable_set('chapter_board_pre_tenure_days', $form_state['values']['roles']['chapter_board_pre_tenure_days']);
  variable_set('chapter_board_post_tenure_days', $form_state['values']['roles']['chapter_board_post_tenure_days']);

  // Other fields require special handling
  //
  // Update role names

  $rebuild   = FALSE;
  $positions = _chapter_board_positions();
  $fields    = array('chapter_chair', 'chapter_treasurer', 'chapter_editor',
                     'chapter_secretary', 'chapter_education', 'chapter_webmaster',
                     'chapter_programs', 'chapter_board', 'chapter_other_1',
                     'chapter_other_2', 'chapter_other_3', 'chapter_other_4',
  					 'chapter_other_5', 'chapter_active_member');

  foreach($fields as $field) {

    if (isset($positions[$field])) {
      // Did it change

      $new_name = check_plain($form_state['values']['roles'][$field]);
      $old_name = variable_get($field, $positions[$field]['name']);

      if ($old_name != $new_name) {
        // Update the role name

        $role = user_role_load_by_name($old_name);

        if (($role == FALSE) || empty($old_name)) {
          // Adding a role

          watchdog('chapter', 'Role ' . $new_name . ' not found, adding it');

          $role = new stdClass();
          $role->name = check_plain($new_name);

          user_role_save($role);

          if (!empty($positions[$field]['permissions'])) {

            watchdog('chapter', 'Assigning permissions');
            _chapter_assign_permissions($new_name, $positions[$field]['permissions']);
            $rebuild = TRUE;
          }
        }
        else if (empty($new_name)) {

          watchdog('chapter', 'Deleting role: ' . $old_name);

          // removing a role
          user_role_delete($old_name);
          $rebuild = TRUE;
        }
        else {
          // Just changing an existing name
          $role->name = $new_name;
          user_role_save($role);
        }
      }
      // Always save the variable value off in case it did not exist and have
      // been running with the default

      variable_set($field, $new_name);
    }
  }
  // Rebuild permissions if needed

  if ($rebuild) {
    watchdog('chapter', 'Performing node rebuild to pick up permissions of new ' .
    					'or deleted roles.');

    node_access_rebuild(TRUE);
  }
}

/**
 * Validate chapter settings menu input
 *
 * This function is called to validate user input to the 'chapter_admin'
 * settings menu.
 *
 * @param $form
 * @param $form_state
 */
function chapter_admin_validate($form, &$form_state) {
  watchdog('chapter', 'entered chapter_admin_validate');

 // $db_integrity  = $form_state['values']['chapter_db_integrity'];
  $evaluate_this = TRUE;

  if (!$evaluate_this) {
    form_set_error('chapter_db_integrity', t('You made a boo boo.'));
  }
}

/**
 * Helper validation functions
 */
function _chapter_validate_integer_positive_or_blank($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be greater than or equal to zero (or blank).', array('%name' => $element['#title'])));
  }
}
