// $Id$

Readme file for the Non-Profit Chapter Tools series of modules for Drupal
---------------------------------------------

The Chapter modules are a collection of Drupal 7.x modules that are intended
to assist in the creation and maintenance of websites for chapters of
the American Wine Society.

The intent is to create a series of modules that assist users with little
or no web server/IT knowledge to maintain a website that services members
of a local non-profit organization.  While the original target audience for
these modules is a particular AWS Chapter (http://naaws-hsv.com), many of the
chapter specific titles/policies are configurable.  In fact, these modules
are even being targeted at assisting other chapters of non-profits to support
their membership needs.

Installation:
  Installation is like with all normal drupal modules:

  - Extract the 'chapter' folder from the tar-ball/zip-file to the
    modules directory from your website (typically sites/all/modules).

Dependencies:
  Review the README.txt in each of the module subdirectories for the latest
  dependency information.  All submodules will require aws-core to be
  installed.

Conflicts/known issues:
  None at this time.  Note that many of the submodules use or extend the basic
  functionality of other Drupal modules and these may have known conflicts or
  issues opened.

Configuration:
  Individual modules are enabled from the module list/configuration page
  at admin/modules/list.  Many submodule will have additionally have permission
  and configuration capabilities and these can be accessed from the module's
  admin/config/chapter page.

Disclaimer:
  These modules were created by an independent developer for the purposes of
  automating the monthly chores in running a chapter.  The National AWS
  officers and membership has not endorsed this project and does not bear any
  liability for it's use or misuse by adopters.  When you think of it, this
  is completely free and open source module anyway and I encourage you to
  review the source and provide useful comments.  Any additional features,
  bug reports, or use-cases can only make the module better and more useful
  to other non-profits that may work with it.
  
  Should you use these modules for a basis for a commercial site, please take
  the time to make a small donation to a local non-profit organization in your
  area.
