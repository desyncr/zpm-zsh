After being disabled, plugin is not listed anymore
  $ zpm reset
  $ zpm "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm disable "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm list
  Nothing to show.
  [1]

Plugin directory is not removed after disable
  $ zpm reset
  $ zpm "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm disable "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ ls $ZPM_TEST_REPOS
  example-plugin
  .* (re)

Plugin can be properly enabled/installed after "disable"
  $ zpm reset
  $ zpm "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm disable "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm "$ZPM_TEST_REPOS/example-plugin" > /dev/null
  $ zpm list
  .*example-plugin .* (re)


