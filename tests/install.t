# vim: set ts=2 sw=2 expandtab:

Can install multiple plugins under the same user.

  $ zpm reset
  $ zpm "$ZPM_TEST_REPOS/example-plugin"
  Installing .*/example-plugin... Done. (re)
  $ zpm "$ZPM_TEST_REPOS/example-theme"
  Installing .*/example-theme... Done. (re)
  $ ls $ZPM_TEST_REPOS
  example-plugin (re)
  example-theme (re)

Avoid installing plugin twice
  $ zpm reset
  $ zpm "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm "$ZPM_TEST_REPOS/example-plugin"
  Plugin ".*/example-plugin" already installed. (re)
  [1]

