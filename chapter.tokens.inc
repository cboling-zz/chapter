<?php
// $Id: $
/**
 * @file
 *
 * Contains token callback support for the core chapter entities.  The
 * main motivations for token support in the chapter module is to allow
 * for automation of event and newsletter mailings.  As the module matures
 * and changes over time, the tokens will provide a layer of abstraction
 * to the underlying database implementation.
 */
/**
 * Implements hook_token_info()
 *
 * Provide information about available placeholder tokens and token types.
 *
 * Tokens are placeholders that can be put into text by using the syntax
 * [type:token], where type is the machine-readable name of a token type,
 * and token is the machine-readable name of a token within this group.
 * This hook provides a list of types and tokens to be displayed on text
 * editing screens, so that people editing text can see what their token
 * options are.
 *
 * The actual token replacement is done by token_replace(), which invokes
 * hook_tokens(). Your module will need to implement that hook in order to
 * generate token replacements from the tokens defined here.
 *
 * The token types defined in the core module are:
 *
 *    'contact'		People/contacts
 *    'electronic'	Electronic Addresses
 *    'location'	Physical Location Information
 *    'membership'	Membership Information
 *    'chapter'		Chapter Information
 *    'board'		Chapter Board Information
 */
function chapter_token_info() {
  //////////////////////////////////////////////////
  // Contact content-type group
  $types['contact'] = array(
    'name' => t("Chapter Contacts"),
    'description' => t("Tokens related to chapter contact fields."),
    'needs-data' => 'contact',
  );
  $contact['description'] = array(
    'name'        => t("Short Description"),
    'description' => t("The title/short-description for the contact."),
  );
  $contact['notes'] = array(
    'name'        => t("Notes"),
    'description' => t("The note/body text for the contact."),
  );
  $contact['title'] = array(
    'name'        => t("Title"),
    'description' => t("The title/salutation for the contact."),
  );
  $contact['first-name'] = array(
    'name'        => t("First Name"),
    'description' => t("The first name of the contact."),
  );
  $contact['middle-name'] = array(
    'name'        => t("Middle Name"),
    'description' => t("The middle name of the contact."),
  );
  $contact['last-name'] = array(
    'name'        => t("Last Name"),
    'description' => t("The last name of the contact."),
  );
  $contact['suffix'] = array(
    'name'        => t("Suffix"),
    'description' => t("The suffix (Jr, III, ...) for the contact."),
  );
  $contact['nickname'] = array(
    'name'        => t("Nickname"),
    'description' => t("The nickname for the contact."),
  );
  $contact['full-name'] = array(
    'name'        => t("Full Name"),
    'description' => t("The full (formal) name for the contact."),
  );
  $contact['short-name'] = array(
    'name'        => t("Short Name"),
    'description' => t("A shorter, less formal. name for the contact."),
  );
  $contact['home-number'] = array(
    'name'        => t("Home Number"),
    'description' => t("Home phone number for the contact."),
  );
  $contact['work-number'] = array(
    'name'        => t("Work Number"),
    'description' => t("Work phone number for the contact."),
  );
  $contact['cell-number'] = array(
    'name'        => t("Cell Number"),
    'description' => t("Mobile/cell phone number for the contact."),
  );
  $contact['fax-number'] = array(
    'name'        => t("FAX Number"),
    'description' => t("Facsimile phone number for the contact."),
  );
  $contact['loc-nid-1'] = array(
    'name'        => t("Primary Location"),
    'description' => t("Primary Physical Location Node ID for the contact."),
    );
  $contact['loc-nid-2'] = array(
    'name'        => t("Secondary Location"),
    'description' => t("Secondary Physical Location Node ID for the contact."),
    );
  $contact['elec-nid-1'] = array(
    'name'        => t("Primary Electronic Address"),
    'description' => t("Primary Electronic Address Node ID for the contact."),
  );
  $contact['elec-nid-2'] = array(
    'name'        => t("Secondary Electronic Address"),
    'description' => t("Secondary Electronic Address Node ID for the contact."),
  );
  $contact['membership-nid'] = array(
    'name'        => t("Chapter Membership"),
    'description' => t("Chapter Membership Node ID for the contact."),
  );
  $contact['uid'] = array(
    'name'        => t("User ID"),
    'description' => t("Account/user-id for the contact"),
  );
  //////////////////////////////////////////////////
  // Electronic Information content-type group

  $types['electronic'] = array(
    'name' => t("Electronic Addresses"),
    'description' => t("Tokens related to electronic addresses."),
    'needs-data' => 'chapter',
  );
  $elec['title'] = array(
    'name'        => t("Title"),
    'description' => t("The short title for the electronic address."),
  );
  $elec['notes'] = array(
    'name'        => t("Notes"),
    'description' => t("The note/body text for the electronic address."),
  );
  $elec['email'] = array(
    'name'        => t("Email"),
    'description' => t("Email Address link/URL"),
  );
  $elec['www'] = array(
    'name'        => t("Web Page"),
    'description' => t("The Web Page link/URL"),
  );
  $elec['sms'] = array(
    'name'        => t("SMS Number"),
    'description' => t("The SMS Number"),
  );
  //////////////////////////////////////////////////
  // Physical Location content-type group

  $types['location'] = array(
    'name' => t("Physical Location Information"),
    'description' => t("Tokens related to location."),
    'needs-data' => 'chapter',
  );
  $local['title'] = array(
    'name'        => t("Title"),
    'description' => t("The title for the location."),
  );
  $local['notes'] = array(
    'name'        => t("Notes"),
    'description' => t("The note/body text for the location."),
  );
  $local['premise'] = array(
    'name'        => t("Primary Address"),
    'description' => t("Primary address line for the location."),
  );
  $local['subpremise'] = array(
    'name'        => t("Secondary Address"),
    'description' => t("Secondary address line for the location."),
  );
  $local['locality'] = array(
    'name'        => t("Locality/City"),
    'description' => t("The city or township for the location."),
  );
  $local['state'] = array(
    'name'        => t("State"),
    'description' => t("The state or province for the location."),
  );
  $local['country'] = array(
    'name'        => t("Country"),
    'description' => t("The country for the location."),
  );
  $local['postal-code'] = array(
    'name'        => t("Postal Code"),
    'description' => t("The Postal/Zip code for the location."),
  );
  $local['directions'] = array(
    'name'        => t("Directions"),
    'description' => t("The directions to find the location."),
  );
  //////////////////////////////////////////////////
  // Membership content-type group

  $types['membership'] = array(
    'name' => t("Membership Information"),
    'description' => t("Tokens related to membership."),
    'needs-data' => 'chapter',
  );
  $member['title'] = array(
    'name'        => t("Title"),
    'description' => t("The title for the membership."),
  );
  $member['notes'] = array(
    'name'        => t("Notes"),
    'description' => t("The note/body text for the membership."),
  );
  $member['id'] = array(
    'name'        => t("Membership ID"),
    'description' => t("The membership identification for this entry."),
  );
  $member['join-date'] = array(
    'name'        => t("Join Date"),
    'description' => t("The date that this membership became active."),
  );
  $member['join-accurate'] = array(
    'name'        => t("Join Accurate"),
    'description' => t("Flag indicating if the join date should be considered accurate."),
  );
  $member['type'] = array(
    'name'        => t("Membership Type"),
    'description' => t("Taxonomy fields describing the type/class of membership."),
  );
  $member['chapter-nid'] = array(
    'name'        => t("Chapter Node ID"),
    'description' => t("The chapter node ID that this membership belongs to."),
  );
  //////////////////////////////////////////////////
  // Chapter content-type group

  $types['chapter'] = array(
    'name' => t("Chapter Information"),
    'description' => t("Tokens related to chapters."),
    'needs-data' => 'chapter',
  );
  $chapter['title'] = array(
    'name'        => t("Title"),
    'description' => t("The title for the chapter."),
  );
  $chapter['notes'] = array(
    'name'        => t("Notes"),
    'description' => t("The note/body text for the chapter."),
  );
  $chapter['active'] = array(
    'name'        => t("Active"),
    'description' => t("Flag indicating if the chapter is currently active."),
  );
  $chapter['board-nid'] = array(
    'name'        => t("Board Node ID"),
    'description' => t("Node ID for the current board associated with this chapter"),
  );
  //////////////////////////////////////////////////
  // Chapter board content-type group

  $types['board'] = array(
    'name' => t("Chapter Board Information"),
    'description' => t("Tokens related to a chapter's board."),
    'needs-data' => 'chapter',
  );
  $board['title'] = array(
    'name'        => t("Title"),
    'description' => t("The title for the board."),
  );
  $board['notes'] = array(
    'name'        => t("Notes"),
    'description' => t("The note/body text for the board."),
  );
  $board['tenure-start'] = array(
    'name'        => t("Start Date"),
    'description' => t("Start date that this board assumed it's responsibilities."),
  );
  $board['tenure-end'] = array(
    'name'        => t("End Date"),
    'description' => t("End date that this board's responsibilities ended."),
  );
  $board['chair'] = array(
    'name'        => t("Chairperson"),
    'description' => t("The chairperson for this board."),
  );
  $board['treasurer'] = array(
    'name'        => t("Treasurer"),
    'description' => t("The treasurer for this board."),
  );
  $board['editor'] = array(
    'name'        => t("Editor"),
    'description' => t("The newsletter editor for this board."),
  );
  $board['secretary'] = array(
    'name'        => t("Secretary"),
    'description' => t("The secretary for this board."),
  );
  $board['education'] = array(
    'name'        => t("Education Chair"),
    'description' => t("The education chair for this board."),
  );
  $board['webmaster'] = array(
    'name'        => t("Webmaster"),
    'description' => t("The webmaster for this board."),
  );
  $board['programs'] = array(
    'name'        => t("Program's Chair"),
    'description' => t("The programs manager for this board."),
  );
  // Now return our token information

  return array(
    'types'  => $types,
    'tokens' => array(
      'contact'    => $contact,
      'electronic' => $elec,
      'location'   => $local,
      'membership' => $member,
      'chapter'    => $chapter,
      'board'      => $board,
    ),
  );
}

/**
 * Implements hook_tokens()
 *
 * Provide replacement values for placeholder tokens
 *
 * @param $type    The machine-readable name of the type (group) of token being
 * 				   replaced, such as 'node', 'user', or another type defined by
 * 				   a hook_token_info() implementation.
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function chapter_tokens($type, $tokens, array $data = array(), array $options = array()) {
  // Each content type handles it's own

  switch ($type) {
    case 'contact':
      return _chapter_contact_token($tokens, $data, $options);
    case 'electronic':
      return _chapter_electronic_token($tokens, $data, $options);
    case 'location':
      return _chapter_location_token($tokens, $data, $options);
    case 'membership':
      return _chapter_membership_token($tokens, $data, $options);
    case 'chapter':
      return _chapter_chapter_token($tokens, $data, $options);
    case 'board':
      return _chapter_board_token($tokens, $data, $options);
  }
}

/**
 * Provide replacement values for contact placeholder tokens
 *
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function _chapter_contact_token($tokens, $data, $options) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data['contact'])) {

    $object = $data['contact'];

    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'description':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
        case 'title':
        case 'first-name':
        case 'middle-name':
        case 'last-name':
        case 'suffix':
        case 'nickname':
        case 'full-name':
        case 'short-name':
        case 'home-number':
        case 'work-number':
        case 'cell-number':
        case 'fax-number':
          $replacements[$original] = $sanitize ? check_plain($object->x)
                                               : $object->x;
          break;
        case 'notes':
          $item = $comment->comment_body[LANGUAGE_NONE][0];
          $instance = field_info_instance('node', 'body', 'chapter_contact');
          $replacements[$original] = $sanitize ? _text_sanitize($instance, LANGUAGE_NONE, $item, 'value')
                                               : $item['value'];
          break;
        case 'loc-nid-1':
        case 'loc-nid-2':
        case 'elec-nid-1':
        case 'elec-nid-2':
        case 'membership-nid':
          if (!empty($object->x)) {
            $node = node_load($object->x);
            if (!empty($node)) {
              $title = $node->title;
              $replacements[$original] = $sanitize ? filter_xss($title) : $title;
            }
          }
          break;
        case 'uid':
          $replacements[$original] = $object->x;
          break;
      }
    }
    // Chained token relationships

    // TODO: Any to do here for this content type
  }
  return $replacements;
}

/**
 * Provide replacement values for electronic placeholder tokens
 *
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function _chapter_electronic_token($tokens, $data, $options) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data['electronic'])) {

    $object = $data['electronic'];

    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'title':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
        case 'notes':
          $item = $comment->comment_body[LANGUAGE_NONE][0];
          $instance = field_info_instance('node', 'body', 'chapter_electronic');
          $replacements[$original] = $sanitize ? _text_sanitize($instance, LANGUAGE_NONE, $item, 'value')
                                               : $item['value'];
          break;
        case 'email':
        case 'www':
        case 'sms':
          $replacements[$original] = $sanitize ? check_plain($object->x)
                                               : $object->x;
          break;
      }
    }
    // Chained token relationships

    // TODO: Any to do here for this content type
  }
  return $replacements;
}

/**
 * @param $type    The machine-readable name of the type (group) of token being
 * 				   replaced, such as 'node', 'user', or another type defined by
 * 				   a hook_token_info() implementation.
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function _chapter_location_token($tokens, $data, $options) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data['location'])) {

    $object = $data['location'];

    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'title':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
        case 'notes':
          $item = $comment->comment_body[LANGUAGE_NONE][0];
          $instance = field_info_instance('node', 'body', 'chapter_location');
          $replacements[$original] = $sanitize ? _text_sanitize($instance, LANGUAGE_NONE, $item, 'value')
                                               : $item['value'];
          break;
        case 'premise':
        case 'subpremise':
        case 'locality':
        case 'state':
        case 'country':
        case 'postal-code':
          $replacements[$original] = $sanitize ? check_plain($object->x)
                                               : $object->x;
          break;
        case 'directions':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
      }
    }
    // Chained token relationships

    // TODO: Any to do here for this content type
  }
  return $replacements;
}

/**
 * Provide replacement values for placeholder tokens
 *
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function _chapter_membership_token($tokens, $data, $options) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data['membership'])) {

    $object = $data['membership'];

    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'title':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
        case 'notes':
          $item = $comment->comment_body[LANGUAGE_NONE][0];
          $instance = field_info_instance('node', 'body', 'chapter_membership');
          $replacements[$original] = $sanitize ? _text_sanitize($instance, LANGUAGE_NONE, $item, 'value')
                                               : $item['value'];
          break;
        case 'join-date':
          $replacements[$original] = format_date($object->x, 'medium', '', NULL, $language_code);
          break;
        case 'join-accurate':
        case 'type':
        case 'id':
          $replacements[$original] = $sanitize ? check_plain($object->x)
                                               : $object->x;
          break;
        case 'chapter-nid':
          if (!empty($object->x)) {
            $node = node_load($object->x);
            if (!empty($node)) {
              $title = $node->title;
              $replacements[$original] = $sanitize ? filter_xss($title) : $title;
            }
          }
          break;
       }
    }
    // Chained token relationships

    // TODO: Any to do here for this content type.  Perhaps the date format?
  }
  return $replacements;
}

/**
 * Provide replacement values for chapter placeholder tokens
 *
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function _chapter_chapter_token($tokens, $data, $options) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data['chapter'])) {

    $object = $data['chapter'];

    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'title':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
        case 'notes':
          $item = $comment->comment_body[LANGUAGE_NONE][0];
          $instance = field_info_instance('node', 'body', 'chapter_chapter');
          $replacements[$original] = $sanitize ? _text_sanitize($instance, LANGUAGE_NONE, $item, 'value')
                                               : $item['value'];
          break;
        case 'active':
          $replacements[$original] = $sanitize ? check_plain($object->x)
                                               : $object->x;
          break;
        case 'board-nid':
          if (!empty($object->x)) {
            $node = node_load($object->x);
            if (!empty($node)) {
              $title = $node->title;
              $replacements[$original] = $sanitize ? filter_xss($title) : $title;
            }
          }
          break;
      }
    }
    // Chained token relationships

    // TODO: Any to do here for this content type
  }
  return $replacements;
}

/**
 * Provide replacement values for chapter board placeholder tokens
 *
 * @param $tokens  An array of tokens to be replaced. The keys are the machine-
 * 				   readable token names, and the values are the raw [type:token]
 * 				   strings that appeared in the original text.
 * @param $data    (optional) An associative array of data objects to be used
 * 				   when generating replacement values, as supplied in the $data
 * 				   parameter to token_replace().
 * @param $options (optional) An associative array of options for token
 * 				   replacement; see token_replace() for possible values.
 *
 * @return An associative array of replacement values, keyed by the raw
 * 		   [type:token] strings from the original text.
 */
function _chapter_board_token($tokens, $data, $options) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data['board'])) {

    $object = $data['board'];

    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'title':
          $replacements[$original] = $sanitize ? filter_xss($object->x)
                                               : $object->x;
          break;
        case 'notes':
          $item = $comment->comment_body[LANGUAGE_NONE][0];
          $instance = field_info_instance('node', 'body', 'chapter_board');
          $replacements[$original] = $sanitize ? _text_sanitize($instance, LANGUAGE_NONE, $item, 'value')
                                               : $item['value'];
          break;
        case 'tenure-start':
          $replacements[$original] = format_date($object->x, 'medium', '', NULL, $language_code);
          break;
        case 'tenure-end':
          $replacements[$original] = format_date($object->x, 'medium', '', NULL, $language_code);
          break;
        case 'chair':
        case 'treasurer':
        case 'editor':
        case 'secretary':
        case 'education':
        case 'webmaster':
        case 'programs':
          $replacements[$original] = $object->x;
          break;
      }
    }
    // Chained token relationships

    // TODO: Any to do here for this content type.  Perhaps the date format?
  }
  return $replacements;
}
