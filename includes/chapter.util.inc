<?php
// $Id: $
/**
 * @file
 *
 * Contains utility routines for the Chapter module
 */

/**
 * Define a few default board position names.  These may be updated after install
 * but during install, these values are also may be used to create a set of roles.
 * After installation, changes in the board position names will show up on any
 * nodes or forms, but the permissions 'name' will remain
 * See the 'CHAPTER_ASSIGN_ROLES_ON_INSTALL' variable for more information.
 */
define('CHAPTER_CHAIR_DEFAULT',     'Director');          // Chairperson.  Head honcho
define('CHAPTER_TREASURER_DEFAULT', 'Treasurer');         // Handles finances
define('CHAPTER_EDITOR_DEFAULT',    'Editor');            // Edits newsletters and content
define('CHAPTER_SECRETARY_DEFAULT', 'Secretary');         // Maintains minutes and handles correspondence
define('CHAPTER_EDUCATION_DEFAULT', 'Education');         // In charge of educational activities
define('CHAPTER_WEBMASTER_DEFAULT', 'Webmaster');         // Manages the website
define('CHAPTER_PROGRAMS_DEFAULT',  'Programs');          // Manages/co-ordinates other activities
define('CHAPTER_BOARD_MEMBER_DEFAULT',  'Board Member');  // General board member.
define('CHAPTER_ACTIVE_MEMBER_DEFAULT', 'Active Member'); // Non-board member (active in chapter)

define('CHAPTER_UNASSIGNED_POSITION', '');

define('CHAPTER_FK_DELETE_RESTRICT', 0);
define('CHAPTER_FK_DELETE_NULLIFY',  1);
define('CHAPTER_FK_DELETE_IGNORE',   2);

/**
 * Variables used by this module
 *
 * This function returns a list of module variables and is called by Installation,
 * configuration, andn testing to name a few.
 */
function _chapter_installed_variables() {
  // Use get_t() to get the name of our localization function for translation
  // during install, when t() is not available.

  $t = get_t();

  $variables = array(
    //////////////////////////////////////////////////////////////////////
    // Internal variables
    //
    // These variables are for internal use and not exposed to the outside world
    //
    'chapter_member_type_vocabulary' => array('test' => 'is_numeric', 'default' => 0),

    // Node type export weights. The actual value (default) is set by a different function.

    'chapter_node_type_weights' => array('test' => 'is_array', 'default' => array()),

    //////////////////////////////////////////////////////////////////////
    // Database options
    //
    // If 'db_integrity' is true, then attempts to prevent deletion of relational
    // database information is enforced where possible.  For instance, if a
    // contact references a location node, the location node cannot be deleted.
    // All contacts, events, ... that references that location must first
    // be deleted.  For some nodes, the last deletion of a node that references
    // that particular node will result in it's deletion.   See the description
    // of each node type for a definition of how referential integrity is
    // applied to that node.
    //
    // When a referenced node is deleted, nodes that reference them may operate
    // on their reference based on the global 'fk_opitons'.  The allowed values
    // are:
    //      - restrict You cannot delete the referenced node until all other
    //                 nodes referencing them are delted or the key set to NULL
    //      - nullify  Automatically set key in the nodes referencing the deleted
    //                 node to NULL.   The delete is allowed.
    //      - ignore   Do not update the key in the referencing node and allow the
    //                 delete.

    'chapter_db_integrity'  => array('test' => 'is_bool',   'default' => TRUE),
    'chapter_db_fk_options' => array('test' => 'is_string', 'default' => CHAPTER_FK_DELETE_IGNORE),

    //////////////////////////////////////////////////////////////////////
    // Administrative 'Board Member' role names.
    //
    // If 'local_dues' is true, then people must pay the indicated amount
    // during to be considered an 'active' member of the local chapter.  If
    // National dues are required, they are administered/verified elsewhere.
    //
    // The 'symbol' should be a valid HTML symbol/character such a '&euro;'

    'chapter_local_dues'          => array('test' => 'is_bool',    'default' => FALSE),
    'chapter_local_dues_amount'   => array('test' => 'is_numeric', 'default' => 0),
    'chapter_local_dues_currency' => array('test' => 'is_string',  'default' => $t('dollars')),
    'chapter_local_dues_symbol'   => array('test' => 'is_string',  'default' => $t('$')),

    //////////////////////////////////////////////////////////////////////
    // Event policies
    //
    // o The Guest Limit is the maximum number of times an individual guest
    //   can attend a chapter sponsered event before they must decide to
    //   become a member.  Set to zero to disable this limit check.
    //
    // o The active limit is the maximum number of days that is allowed to
    //   go by before declaring a member as 'inactive'.  Besides possibly
    //   needing to pay national or local dues, the 'active' limit can be
    //   used to automatically age off people who may no longer belong.
    //   Active members are a special role and may be assigned more permissions
    //   that just an authenticated user.  Set to zero to disable this feature.

    'chapter_guest_event_limit' => array('test' => 'is_int', 'default' => 3),
    'chapter_active_limit'      => array('test' => 'is_int', 'default' => 365),

    //////////////////////////////////////////////////////////////////////
    // Active member role name.
    //
    // To encourage event attendance (or paying of dues) an additional role
    // of 'active member' is defined that can be provided with additional
    // access rights to some content and site features.  All 'active member's
    // are 'authenticated user's but not all 'authenticated user' are
    // active members of the chapter.

    'chapter_active_member' => array('test' => 'is_string', 'default' => $t('Active Member')),

    //////////////////////////////////////////////////////////////////////
    // Administrative 'Board Member' role names.
    //
    // If value is NULL or blank, the position is not used.  Roles may be
    // assigned to multiple user accounts.  For instance, if you have a
    // director and a director-elect, just assign them both to 'director'.
    // The directory elect will fill in for the directory when they are not
    // available and will most likely need the same access privledges.
    //
    // Also, one person can assume multiple roles...
    //
    // Setting any entry to an empty string or NULL disables that role.
    //

    'chapter_chair'     => array('test' => 'is_string', 'default' => $t(CHAPTER_CHAIR_DEFAULT)),
    'chapter_treasurer' => array('test' => 'is_string', 'default' => $t(CHAPTER_TREASURER_DEFAULT)),
    'chapter_editor'    => array('test' => 'is_string', 'default' => $t(CHAPTER_EDITOR_DEFAULT)),
    'chapter_secretary' => array('test' => 'is_string', 'default' => $t(CHAPTER_SECRETARY_DEFAULT)),
    'chapter_education' => array('test' => 'is_string', 'default' => $t(CHAPTER_EDUCATION_DEFAULT)),
    'chapter_webmaster' => array('test' => 'is_string', 'default' => $t(CHAPTER_WEBMASTER_DEFAULT)),
    'chapter_programs'  => array('test' => 'is_string', 'default' => $t(CHAPTER_PROGRAMS_DEFAULT)),
    'chapter_board'     => array('test' => 'is_string', 'default' => $t(CHAPTER_BOARD_MEMBER_DEFAULT)),

    // The following are customizable in case you have additional rols
    // that need filling and need different access permissions than any
    // of the pre-defined roles above.

    'chapter_other_1' => array('test' => 'is_string', 'default' => $t(CHAPTER_UNASSIGNED_POSITION)),
    'chapter_other_2' => array('test' => 'is_string', 'default' => $t(CHAPTER_UNASSIGNED_POSITION)),
    'chapter_other_3' => array('test' => 'is_string', 'default' => $t(CHAPTER_UNASSIGNED_POSITION)),
    'chapter_other_4' => array('test' => 'is_string', 'default' => $t(CHAPTER_UNASSIGNED_POSITION)),
    'chapter_other_5' => array('test' => 'is_string', 'default' => $t(CHAPTER_UNASSIGNED_POSITION)),

    // To facilitate board transitions, a period of time before and after the
    // tenure period allows a former (or new) board member read access to
    // board data.  This value is in 'days'

    'chapter_board_pre_tenure_days'  => array('test' => 'is_int', 'default' => 0),
    'chapter_board_post_tenure_days' => array('test' => 'is_int', 'default' => 0),

    //////////////////////////////////////////////////////////////////////
    // Debugging options
    //
    // The following are to assist during debugging of install/uninstall so
    // that some content and roles are not removed.

    'chapter_retain_content'       => array('test' => 'is_bool', 'default' => FALSE),
    'chapter_retain_content_types' => array('test' => 'is_bool', 'default' => FALSE),
    'chapter_retain_roles'         => array('test' => 'is_bool', 'default' => FALSE),
    'chapter_retain_variables'     => array('test' => 'is_bool', 'default' => FALSE),

    //////////////////////////////////////////////////////////////////////
    // Misc. options
    //
    // Next membership_id is used to seed a table that automatically creates
    // a membership id if one is not provided.  Each get will increment the
    // id number.  It is not thread safe but it is not expected that multiple
    // contacts will be added multiple people at the same time.  For some
    // installations, you may not want the ID to be unique so that you can
    // assign a single ID to both spouse or an entire family...
    //
    // If auto-member-id is 'FALSE', an administrator must manually enter
    // membership IDs.  Leave it at 'FALSE' if you do not care about
    // membership IDs.
    //
    // If hide-add-content is 'TRUE' the content types for this module will
    // be hidden from the 'add content' type menu and only the installed web
    // forms can be used to add our content types.  Roles with explicit
    // 'administer nodes' rights always can use the 'add content' menu method
    // of inserting content.
    //
    // If use-relations is 'TRUE' then the 'relations' module will be used
    // instead of the 'reference' (user & node references).  Currently, if you
    // change this value, you must first disable and 'uninstall' the 'chapter'
    // module and then re-install.

    'chapter_next_member_id'   => array('test' => 'is_int',  'default' => 2),
    'chapter_auto_member_id'   => array('test' => 'is_bool', 'default' => FALSE),
    'chapter_hide_add_content' => array('test' => 'is_bool', 'default' => TRUE),
    'chapter_use_relations'    => array('test' => 'is_bool', 'default' => FALSE),
  );
  return $variables;
}

/**
 * Install common (core) field types used throughout the 'chapter' suite of
 * modules
 */
function _chapter_installed_fields() {
  $t             = get_t();
  $use_relations = variable_get('chapter_use_relations', FALSE);

  // Roles allowed is everyone except anonymous users.

  $auth_roles = array();
  $auth_roles[DRUPAL_AUTHENTICATED_RID] = DRUPAL_AUTHENTICATED_RID;

  //  Now create our fields

  $fields = array(
    ///////////////////////////////////////////
    // Common fields
    ///////////////////////////////////////////
    // Contact related fields
    'chapter_contact_title' => array(
      'field_name'   => 'chapter_contact_title',
      'label'        => $t(''),
      'type'         => 'text',
      'translatable' => TRUE,
      'description'  => $t('Name prefix or '),
      'settings'     => array('max_length'   => 16,
                              'display_size' => 8),
    ),
    'chapter_contact_first_name' => array(
      'field_name'   => 'chapter_contact_first_name',
      'label'        => $t('First Name'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('First Name'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 12),
    ),
    'chapter_contact_middle_name' => array(
      'field_name'   => 'chapter_contact_middle_name',
      'label'        => $t('Middle Name or Initial'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('First, Middle, Last, or Nickname'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 8),
    ),
    'chapter_contact_last_name' => array(
      'field_name'   => 'chapter_contact_last_name',
      'label'        => $t('Last Name'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('First, Middle, Last, or Nickname'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 16),
    ),
    'chapter_contact_nickname' => array(
      'field_name'   => 'chapter_contact_nickname',
      'label'        => $t('Nickname'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('Nickname'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 12),
    ),
    'chapter_contact_suffix' => array(
      'field_name'   => 'chapter_contact_suffix',
      'label'        => $t('Suffix'),
      'type'         => 'text',
      'translatable' => TRUE,
      'description'  => $t('Name suffix'),
      'settings'     => array('max_length'   => 32,
                              'display_size' => 8,),
    ),
    'chapter_home_number' => array(
      'field_name'   => 'chapter_home_number',
      'label'        => $t('Home Phone'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('Phone number and extension'),
      'settings'     => array('max_length' => 24,
                              'display_size' => 14,),
    ),
    'chapter_work_number' => array(
      'field_name'   => 'chapter_work_number',
      'label'        => $t('Work Phone'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('Phone number and extension'),
      'settings'     => array('max_length'   => 24,
                              'display_size' => 14,),
    ),
    'chapter_cell_number' => array(
      'field_name'   => 'chapter_cell_number',
      'label'        => $t('Mobile/Cell Phone'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('Phone number and extension'),
      'settings'     => array('max_length'   => 24,
                              'display_size' => 14,),
    ),
    'chapter_fax_number' => array(
      'field_name'   => 'chapter_fax_number',
      'label'        => $t('FAX Phone'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('Phone number and extension'),
      'settings'     => array('max_length'   => 24,
                              'display_size' => 14,),
    ),
    'chapter_user_account' => array(
      'field_name'   => 'chapter_user_account',
      'label'        => $t('User Account'),
      'type'         => $use_relations ? 'relation' : 'user_reference',
      'module'       => $use_relations ? 'relation' : 'user_reference',
      'description'  => $t('Links a contact node with a user entity'),
      'settings'     => array(
        'referenceable_roles'  => $auth_roles,
        'referenceable_status' => array('1', '1'),      // Allow both blocked and active
      ),
    ),
    'chapter_primary_address' => array(
      'field_name'   => 'chapter_primary_address',
      'label'        => $t('Primary Address'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a contact node with a location node'),
      'settings'     => array('referenceable_types' => array('chapter_location'),),
    ),
    'chapter_secondary_address' => array(
      'field_name'   => 'chapter_secondary_address',
      'label'        => $t('Secondary Address'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a contact node with a location node'),
      'settings'     => array('referenceable_types' => array('chapter_location'),),
    ),
    'chapter_primary_email' => array(
      'field_name'   => 'chapter_primary_email',
      'label'        => $t('Primary Email and WWW Addresses'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a contact node with a electronic information node'),
      'settings' => array('referenceable_types' => array('chapter_electronic'),),
    ),
    'chapter_secondary_email' => array(
      'field_name'   => 'chapter_secondary_email',
      'label'        => $t('Secondary Email and WWW Addresses'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a contact node with a electronic information node'),
      'settings' => array('referenceable_types' => array('chapter_electronic'),),
    ),
    'chapter_contact_membership' => array(
      'field_name'   => 'chapter_contact_membership',
      'label'        => $t('Chapter'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a contact node with a membership node'),
      'settings'     => array('referenceable_types' => array('chapter_membership'),),
    ),
    ///////////////////////////////////////////
    // Electronic information related fields
    //
    // NOTE: Will use 3rd party 'link' module field

    'chapter_email_url' => array(
      'field_name'   => 'chapter_email_url',
      'label'        => $t('Email Address'),
      'type'         => 'link_field',
      'translatable' => FALSE,
      'description'  => $t('Label and URL for email address'),
      'settings'     => array(
         'max_length' => 255,
         'title'      => 'optional',
         'url'        => 'required',
         'attributes' => 'rel=&quot;nofollow&quot;',
      ),
    ),
    'chapter_www_url' => array(
      'field_name'   => 'chapter_www_url',
      'label'        => $t('WWW Address'),
      'type'         => 'link_field',
      'translatable' => FALSE,
      'description'  => $t('Label and URL WWW server'),
      'settings'     => array(
         'max_length' => 255,
         'title'      => 'optional',
         'url'        => 'required',
         'attributes' => 'rel=&quot;nofollow&quot;',
      ),
    ),
    'chapter_sms' => array(
      'field_name'   => 'chapter_sms',
      'label'        => $t('SMS Address'),
      'type'         => 'text',
      'translatable' => FALSE,
      'description'  => $t('SMS Address'),
      'settings'     => array('max_length' => 24,),
    ),
    ///////////////////////////////////////////
    // Physical location related fields.  The selection of the field names
    // is to relate to specific elements of the xAL standard that we may
    // wish to support in the future.

    'chapter_premise' => array(
      'field_name'   => 'chapter_premise',
      'label'        => $t('Address'),
      'type'         => 'text',
      'translatable' => TRUE,
      'description'  => $t('Primary address line, xNAL Element: Premise'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 48,),
    ),
    'chapter_subpremise' => array(
      'field_name'   => 'chapter_subpremise',
      'label'        => $t(''),
      'type'         => 'text',
      'translatable' => TRUE,
      'description'  => $t('Secondary address line, xNAL Element: SubPremis'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 48,),
    ),
    'chapter_locality' => array(
      'field_name'   => 'chapter_locality',
      'label'        => $t('City'),
      'type'         => 'text',
      'translatable' => TRUE,
      'description'  => $t('City or town, xNAL Element: AdministrativeArea'),
      'settings'     => array('max_length'   => 64,
                              'display_size' => 48,),
    ),
    'chapter_state' => array(
      'field_name'   => 'chapter_state',
      'label'        => $t('State'),
      'type'         => 'text',
      'size'         => '3',
      'translatable' => TRUE,
      'description'  => $t('Three letter ISO 3166.2 type state or region code, xNAL Element: AdministrativeArea'),
      'settings'     => array('max_length'   => 3,
                              'display_size' => 3,),
    ),
    'chapter_country' => array(
      'field_name'   => 'chapter_country',
      'label'        => $t('Country'),
      'type'         => 'text',
      'size'         => '2',
      'translatable' => TRUE,
      'description'  => $t('Two letter ISO 3166.2 country code, xNAL Element: CountryNameCode'),
      'settings'     => array('max_length'   => 2,
                              'display_size' => 2,),
    ),
    'chapter_postalcode' => array(
      'field_name'   => 'chapter_postalcode',
      'label'        => $t('Postal Code'),
      'type'         => 'text',
      'size'         => '10',
      'translatable' => TRUE,
      'description'  => $t('Postal Code, xNAL Element: PostalCode'),
      'settings'     => array('max_length'   => 10,
                              'display_size' => 10,),
    ),
    'chapter_directions' => array(
      'field_name'   => 'chapter_directions',
      'label'        => $t('Directions'),
      'type'         => 'text_long',
      'size'         => 'normal',   // 16 KB on mySQL
      'translatable' => TRUE,
      'description'  => $t('Provides directions to a specific location'),
      'settings'     => array(
        'max_length'   => 2048,
        'display_size' => 10,
      ),
    ),
    ///////////////////////////////////////////
    // Membership related fields.

    'chapter_membership_id' => array(
      'field_name'   => 'chapter_membership_id',
      'label'        => $t('Membership ID'),
      'type'         => 'text',
      'translatable' => TRUE,
      'description'  => $t('Membership ID for contact'),
      'settings'     => array('max_length' => 32,),
    ),
    'chapter_membership_date' => array(
      'field_name'   => 'chapter_membership_date',
      'label'        => $t('Join Date'),
      'description'  => $t('Month/day/year member belonged'),
      'type'         => 'date',
      'module'       => 'date',
      'translatable' => TRUE,
      'settings'     => array(
        'repeat'      => 0,
        'todate'      => 'optional',
        'tz_handling' => 'none',
        'timezone_db' => '',
        'granularity' => array(
          'year'  => 'year',
          'month' => 'month',
          'day'   => 'day',
        ),
      ),
    ),
    'chapter_membership_date_accurate' => array(
      'field_name'   => 'chapter_membership_date_accurate',
      'label'        => $t('Join Date Accurate'),
      'description'  => $t('Boolean indicating if the join date is accurate ' .
                           'enough for most management purposes'),
      'type'         => 'list_boolean',
      'module'       => 'list',
      'translatable' => TRUE,
      'settings'     => array(
        'allowed_values' => array(0 => '', 1 => ''),
      ),
    ),
    'chapter_membership_type' => array(
      'field_name'   => 'chapter_membership_type',
      'label'        => $t('Membership Type'),
      'description'  => $t('Tracks the current membership type of this contact'),
      'type'         => 'taxonomy_term_reference',
      'cardinality'  => '2',
      'module'       => 'taxonomy',
      'active'       => 1,
      'translatable' => TRUE,
      'settings'     => array(
        'allowed_values' => array(
          array('vocabulary' => 'chapter_member_type', 'parent' => '0'),
        ),
      ),
    ),
    'chapter_membership_chapter' => array(
      'field_name'   => 'chapter_membership_chapter',
      'label'        => $t('Chapter'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a membership node with a chapter node'),
      'settings'     => array('referenceable_types' => array('chapter_chapter'),),
    ),
    ///////////////////////////////////////////
    // Chapter related fields.

    'chapter_chapter_active' => array(
      'field_name'   => 'chapter_chapter_active',
      'label'        => $t('Active'),
      'description'  => $t('Boolean indicating if the chapter is still ' .
                           'active and providing services to members'),
      'type'         => 'list_boolean',
      'module'       => 'list',
      'translatable' => TRUE,
      'settings'     => array(
        'allowed_values' => array(0 => '', 1 => ''),
      ),
    ),
    'chapter_board_current' => array(
      'field_name'   => 'chapter_board_current',
      'label'        => $t('Current Board'),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a board node with a chapter node'),
      'settings'     => array('referenceable_types' => array('chapter_board'),),
    ),
    ///////////////////////////////////////////
    // Chapter Board related fields.

    'chapter_board_tenure' => array(
      'field_name'   => 'chapter_board_tenure',
      'label'        => $t('Tenure Date'),
      'description'  => $t('Start and end (optional) tenure dates for board'),
      'type'         => 'date',
      'module'       => 'date',
      'translatable' => TRUE,
      'settings'     => array(
        'repeat'      => 0,
        'todate'      => 'optional',
        'tz_handling' => 'none',
        'timezone_db' => '',
        'granularity' => array(
          'year'  => 'year',
          'month' => 'month',
          'day'   => 'day',
        ),
      ),
    ),
    'chapter_chair_contact' => array(
      'field_name'   => 'chapter_chair_contact',
      'label'        => variable_get('chapter_chair', CHAPTER_CHAIR_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_treasurer_contact' => array(
      'field_name'   => 'chapter_treasurer_contact',
      'label'        => variable_get('chapter_treasurer', CHAPTER_TREASURER_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_editor_contact' => array(
      'field_name'   => 'chapter_editor_contact',
      'label'        => variable_get('chapter_editor', CHAPTER_EDITOR_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_secretary_contact' => array(
      'field_name'   => 'chapter_secretary_contact',
      'label'        => variable_get('chapter_secretary', CHAPTER_SECRETARY_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_education_contact' => array(
      'field_name'   => 'chapter_education_contact',
      'label'        => variable_get('chapter_education', CHAPTER_EDUCATION_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_webmaster_contact' => array(
      'field_name'   => 'chapter_webmaster_contact',
      'label'        => variable_get('chapter_webmaster', CHAPTER_WEBMASTER_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_programs_contact' => array(
      'field_name'   => 'chapter_programs_contact',
      'label'        => variable_get('chapter_programs', CHAPTER_PROGRAMS_DEFAULT),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_other_1_contact' => array(
      'field_name'   => 'chapter_other_1_contact',
      'label'        => variable_get('chapter_other_1', CHAPTER_UNASSIGNED_POSITION),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_other_2_contact' => array(
      'field_name'   => 'chapter_other_2_contact',
      'label'        => variable_get('chapter_other_2', CHAPTER_UNASSIGNED_POSITION),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_other_3_contact' => array(
      'field_name'   => 'chapter_other_3_contact',
      'label'        => variable_get('chapter_other_3', CHAPTER_UNASSIGNED_POSITION),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_other_4_contact' => array(
      'field_name'   => 'chapter_other_4_contact',
      'label'        => variable_get('chapter_other_4', CHAPTER_UNASSIGNED_POSITION),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
    'chapter_other_5_contact' => array(
      'field_name'   => 'chapter_other_5_contact',
      'label'        => variable_get('chapter_other_', CHAPTER_UNASSIGNED_POSITION),
      'type'         => $use_relations ? 'relation' : 'node_reference',
      'module'       => $use_relations ? 'relation' : 'node_reference',
      'description'  => $t('Links a chapter board member position with a contact'),
      'settings'     => array('referenceable_types' => array('chapter_contact'),),
    ),
  );
  // Done

  return $fields;
}

function _chapter_get_vocabulary() {
  // NOTE: When adding vocabularies, remember to add the vid_variable
  //       name to our list of variables so that is is cleaned up properly
  //       on uninstall

  return array (
    'vocabulary' => array(
      'name'         => 'Member Type',
      'machine_name' => 'chapter_member_type',
      'module'       => 'chapter',
      'description'  => t('Member type description here'),
      'vid_variable' => 'chapter_member_type_vocabulary',

      'terms'        => array(
        array(
          'name'        => t('Active'),
          'parent'      => array(0),
          'description' => t('<p>This term is used to describe a member who is current ' .
          					 'on any dues (if any) with the local and/or national ' .
          					 'organization.</p>' .
                             '<p>Typically <em>active</em> members will have more options ' .
          					 'and privileges with various features on the website.</p>' .
          					 '<p>Optionally, the chapter board or administrator may ' .
          					 'also require a certain level of attendance to qualify as ' .
          					 'an <em>active</em> member.</p>'),
          'format'      => 'filtered_html',
          'vid'         => 0,
          'weight'      => 10,
        ),
        array(
          'name'        => t('Inactive'),
          'parent'      => array(0),
          'description' => t('<p>This term is used to describe a member who is current ' .
          					 'on any dues (if any) with the local and/or national ' .
          					 'organization but may have not met specified attendance ' .
                             'levels in order to be considered an <em>active</em> ' .
                             'member.</p>'),
          'format'      => 'filtered_html',
          'vid'         => 0,
          'weight'      => 20,
        ),
        array(
          'name'        => t('Former'),
          'parent'      => array(0),
          'description' => t('<p>This term is used to describe a former member who ' .
          					 'has not renewed their membership with the local and/or ' .
          					 'national organization.</p>'),
          'format'      => 'filtered_html',
          'vid'         => 0,
          'weight'      => 30,
        ),
        array(
          'name'        => t('Guest'),
          'parent'      => array(0),
          'description' => t('<p>Has attended as a guest of an <em>active</em> member and ' .
          					 'has not yet joined the local and/or national organization.</p>' .
                             '<p>If a member belongs to another chapter, you should enter ' .
                             'them as active and assign them to a different chapter.</p>'),
          'format'      => 'filtered_html',
          'vid'         => 0,
          'weight'      => 40,
        ),
        array(
          'name'        => t('Vendor'),
          'parent'      => array(0),
          'description' => t('<p>A representative of a third-part company or organization.</p>'),
          'format'      => 'filtered_html',
          'vid'         => 0,
          'weight'      => 50,
        ),
        array(
          'name'        => t('Business or Location'),
          'parent'      => array(0),
          'description' => t('<p>A physical business or physical location.</p>' .
                             '<p>Use this term to describe non-person contacts to ' .
                             'track.</p>'),
          'format'      => 'filtered_html',
          'vid'         => 0,
          'weight'      => 60,
        ),
      ),
    ),
  );
}

/**
 * Return an array of export weights with the node-type as the key and the
 * weight as the value.  As the weight increases, more dependencies on lesser
 * weights exists.
 *
 * For the core Chapter module, the following dependencies exist
 *
 *  Node                Depends On
 *  ------------------  ----------------------------------------------------
 *  Electronic Address  -none-
 *  Physical Address    -none-
 *  Chapter             Electronic, Physical
 *  Membership          Chapter
 *  Contact             Membership, Electronic, Physical, User Entity
 *  Board               Contact, Board
 *
 * @return The array is sorted and returned in order of increasing weights
 */
function _chapter_node_export_weights() {
  watchdog('chapter', '_chapter_node_export_weights: entry');
  $node_weights = array(
    'chapter_electronic' => 10,
    'chapter_location'   => 20,
    'chapter_chapter'    => 30,
    'chapter_membership' => 40,
    'chapter_contact'    => 50,
    'chapter_board'      => 60,
  );
  //watchdog('chapter', '_chapter_node_export_weights: before sort');
  asort($node_weights, SORT_NUMERIC);
  //watchdog('chapter', '_chapter_node_export_weights: after sort');

  return $node_weights;
}

function cmp($a, $b)
{
    if ($a == $b) {
        return 0;
    }
    return ($a < $b) ? -1 : 1;
}

/**
 * Compare two node types and determine export weight
 *
 * @return
 *   <0  if a <  b
 *    0  if a == b
 *   >0
 */
function _chapter_node_export_weight_compare($a, $b) {
   if ($a->type == $b->type) {
     return 0;
   }
   $type_weights = variable_get('chapter_node_type_weights', _chapter_node_export_weights());

   $a_weight = empty($type_weights[$a->type]) ? 0 : $type_weights[$a->type];
   $b_weight = empty($type_weights[$b->type]) ? 0 : $type_weights[$b->type];

   return ($a_weight - $b_weight);
}

/**
 * Return an array of the machine_name for board positions
 *
 * Supplimental data includes the following.
 *
 * 'permissions' => Contains the content type positions and whether or not
 *                  the position can 'create', 'edit', and/or 'delete' the
 *                  content type.  The value parameter should be TRUE and if
 *                  not present, it is assumed to be false.  A shortcut
 *                  content type of 'all' indicates that all content type
 *                  operations may be performed.
 */
function _chapter_board_positions() {

  $chair_name     = variable_get('chapter_chair',     t(CHAPTER_CHAIR_DEFAULT));
  $treasurer_name = variable_get('chapter_treasurer', t(CHAPTER_TREASURER_DEFAULT));
  $editor_name    = variable_get('chapter_editor',    t(CHAPTER_EDITOR_DEFAULT));
  $secretary_name = variable_get('chapter_secretary', t(CHAPTER_SECRETARY_DEFAULT));
  $education_name = variable_get('chapter_education', t(CHAPTER_EDUCATION_DEFAULT));
  $webmaster_name = variable_get('chapter_webmaster', t(CHAPTER_WEBMASTER_DEFAULT));
  $programs_name  = variable_get('chapter_programs',  t(CHAPTER_PROGRAMS_DEFAULT));
  $board_name     = variable_get('chapter_board',     t(CHAPTER_BOARD_MEMBER_DEFAULT));
  $active_name    = variable_get('chapter_active_member',  t(CHAPTER_ACTIVE_MEMBER_DEFAULT));

  return array(
     'chapter_chair'     => array(
       'name'        => $chair_name,
       // Chapter chair has full permissions to modify data
       'permissions' => array(
         'chapter_electronic' => array('all' => TRUE),
         'chapter_location'   => array('all' => TRUE),
         'chapter_chapter'    => array('all' => TRUE),
         'chapter_membership' => array('all' => TRUE),
         'chapter_contact'    => array('all' => TRUE),
         'chapter_board'      => array('all' => TRUE),
       ),
     ),
     'chapter_treasurer' => array(
       'name'        => $treasurer_name,
       // Treasurer needs to update the membership status of members
       'permissions' => array(
         'chapter_electronic' => array(),
         'chapter_location'   => array(),
         'chapter_chapter'    => array(),
         'chapter_membership' => array('create' => TRUE, 'update' => TRUE,),
         'chapter_contact'    => array(),
         'chapter_board'      => array(),
       ),
     ),
     'chapter_editor'    => array(
       'name'        => $editor_name,
       // Editor needs to update ways to contact a member and add new ones
       'permissions' => array(
         'chapter_electronic' => array('all' => TRUE),
         'chapter_location'   => array('all' => TRUE),
         'chapter_chapter'    => array(),
         'chapter_membership' => array('create' => TRUE,),
         'chapter_contact'    => array('create' => TRUE, 'update' => TRUE,),
         'chapter_board'      => array(),
       ),
     ),
     'chapter_secretary' => array(
       'name'        => $secretary_name,
       // Secretary is responsible for adding/removing a board after elections
       'permissions' => array(
         'chapter_electronic' => array(),
         'chapter_location'   => array(),
         'chapter_chapter'    => array(),
         'chapter_membership' => array(),
         'chapter_contact'    => array(),
         'chapter_board'      => array('all' => TRUE),
       ),
     ),
     'chapter_education' => array(
       'name'        => $education_name,
       // Education has no role to create, modify, or delete basic core content
       'permissions' => array(),
     ),
     'chapter_webmaster' => array(
       'name'        => $webmaster_name,
       // Webmaster has full permissions to fix everything
       'permissions' => array(
         'chapter_electronic' => array('all' => TRUE),
         'chapter_location'   => array('all' => TRUE),
         'chapter_chapter'    => array('all' => TRUE),
         'chapter_membership' => array('all' => TRUE),
         'chapter_contact'    => array('all' => TRUE),
         'chapter_board'      => array('all' => TRUE),
       ),
     ),
     'chapter_programs'  => array(
       'name'        => $programs_name,
       // Programs needs to be able to update how an existing member can be contacted.
       'permissions' => array(
         'chapter_electronic' => array('all' => TRUE),
         'chapter_location'   => array('all' => TRUE),
         'chapter_chapter'    => array(),
         'chapter_membership' => array(),
         'chapter_contact'    => array('update' => TRUE,),
         'chapter_board'      => array(),
       ),
     ),
     'chapter_board'     => array(
       'name'        => $board_name,
       'permissions' => array(),
     ),
     'chapter_other_1'  => array(
       'name'        => CHAPTER_UNASSIGNED_POSITION,
       'permissions' => array(),
     ),
     'chapter_other_2'  => array(
       'name'        => CHAPTER_UNASSIGNED_POSITION,
       'permissions' => array(),
     ),
     'chapter_other_3'  => array(
       'name'        => CHAPTER_UNASSIGNED_POSITION,
       'permissions' => array(),
     ),
     'chapter_other_4'  => array(
       'name'        => CHAPTER_UNASSIGNED_POSITION,
       'permissions' => array(),
     ),
     'chapter_other_5'  => array(
       'name'        => CHAPTER_UNASSIGNED_POSITION,
       'permissions' => array(),
     ),
     'chapter_active_member' => array(
       'name'        => $active_name,
       'permissions' => array(),
     ),
  );
}

/**
 * Get existing contact nodes
 * This method is used to return a list of existing contact nodes
 * @param unknown_type $conditions
 */
function chapter_get_contact_names($conditions = NULL) {
  // Initially, NID=0 for none-selected
  $names = array(0 => '');

  // Query DB for more contacts and their node IDs

  $args  = array(':type' => 'chapter_contact');
  $query = 'SELECT n.nid, n.title ' .
           'FROM {node} n ' .
           'WHERE n.type = :type';

  // Apply any extra conditions
  // TODO: Need to sort this by title.

  if (!empty($conditions) && is_array($conditions)) {
    foreach ($conditions as $args_val => $query_val) {
      $query += ' AND ' . $query_val;
      $args[] = $args_val;
    }
  }
  // Execute the query

  $result = db_query($query, $args);

  //asort($result, SORT_STRING);

  // Load the results

  foreach ($result as $record) {
    $names[check_plain($record->title) . ' nid:' . $record->nid] = check_plain($record->title);
  }
  return $names;
}