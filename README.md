# daphne-task-manager 2019
- i am writing this C++ task manager for Kiosk

    <div class="row">
      <div class="panel panel-default">
        <div class="panel-heading">
          <h3 class="panel-title"><a id="Features"></a>Features</h3>
        </div>
        <div class="panel-body">
          <ul>
            <li>Main window</li>
            <ul>
              <li>CPU and memory information</li>
              <li>Comprehensive process list</li>
              <li>Access to drag-and-drop tool</li>
            </ul>
            <li>Drag-and-drop tool (Drop tool over any window)</li>
            <ul>
              <li>Find the process which owns a given window</li>
              <li>Find the window in the process windows tree</li>
              <li>Kill the process which owns a given window</li>
              <li>Hide window and remove application from task bar (application keeps running completely hidden, and can be restored later)</li>
              <li>Set or remove always on top property</li>
              <li>Set or remove transparency (alpha)</li>
              <li>Enable or disable window or control</li>
              <li>Change window size using numbers (i.e. 640x480)</li>
              <li>Save window position and size and restore when the process is started</li>
            </ul>
            <li>Process list</li>
            <ul>
              <li>Including CPU usage, PID, full path and arguments, owner, class (process or service), memory and swap usage, I/O count.</li>
              <li>Item highlighting with colors for custom, system and high CPU-consumption processes.</li>
              <li>Sortable by any column</li>
              <li>Quick search-as-you-type find by process name when sorted by Process column</li>
              <li>Customizable column position and width</li>
              <li>Contextual menu allows for:</li>
              <ul>
                <li>Kill the process or ask it to finish execution</li>
                <li>Schedule process kill for later</li>
                <li>Set focus</li>
                <li>Set or remove always on top</li>
                <li>Create a trap</li>
                <li>Change processor affinity mask and optionally trap this configuration</li>
                <li>Change process priority</li>
                <li>Handle service (start, stop, pause and continue)</li>
                <li>Copy process name, PID, path, MD5 or SHA1 to clipboard</li>
                <li>Open containing folder</li>
                <li>Look for or submit to DRK process database.</li>
                <li>Access to process properties window</li>
              </ul>
            </ul>
            <li>Process properties window</li>
            <ul>
              <li>Full command line</li>
              <li>Parent process and user</li>
              <li>Startup timestamp</li>
              <li>Kernel and user time</li>
              <li>Show and handle minimum and maximum working set size</li>
              <li>Windows tree allows for:</li>
              <ul>
                <li>Set focus</li>
                <li>Show or hide window or application</li>
                <li>Handle always on top</li>
                <li>Make window resizable</li>
                <li>Turn caption on and off</li>
                <li>Send a window message (i.e. WM_ACTIVATE) with lParam and wParam values</li>
              </ul>
              <li>Thread list with priority and CPU time</li>
              <li>Loaded modules</li>
              <li>Environment variables</li>
              <li>I/O information</li>
            </ul>
            <li>Daphne main menu (accessible from tray icon too)</li>
            <ul>
              <li>Kill all process by matching name</li>
              <li>Run process as other user</li>
              <li>Close windows by matching title</li>
              <li>Schedule popup message</li>
              <li>Schedule system shutdown</li>
              <li>Access CPU usage graph</li>
              <li>Open process hierarchy tree</li>
              <li><a href="daphne_ss.php#installed">Installed software</a> list for inspecting installed software available in tray icon menu. <a href="daphne_ss.php#installed">Installed software</a></li>
              <li>Control inspector window for revealing hidden passwords and other control information</li>
              <li>Hidden applications and scheduled tasks list</li>
              <li>Copy process list to clipboard</li>
              <li>Windows 7 God Mode and Problem steps recorder</li>
            </ul>
            <li>Traps</li>
            <ul>
              <li>Apply to any process, when its startup is detected.</li>
              <li>Notify process has been created</li>
              <li>Kill it</li>
              <li>Hide the application</li>
              <li>Set always on top on/off</li>
              <li>Set transparency (alpha)</li>
              <li>Set desired priority</li>
              <li>Notify process destroyed/exited</li>
              <li>Set process affinity mask</li>
              <li>Keep window position and size</li>
            </ul>
            <li>Kill menu: User-defined menu items. Every item has a process list and kill them when activated.</li>
            <li>Four user-defined command lines to be executed if Ctrl-Shift-F1 to F4 combination is pressed. Work globally.</li>
            <li>Windows explorer contextual menu integration for files and folders</li>
            <ul>
              <li>Copy name of full path to clipboard</li>
              <li>Copy folder file listing to clipboard</li>
              <li>Open command line (CMD) in folder</li>
              <li>Check which process has a file opened</li>
              <li>Google folder or file name</li>
              <li>Compute MD5 or SHA1 hash, validate and create hash file</li>
            </ul>
            <li>Optional global and customizable activation shortcut for showing Daphne main window</li>
            <li>Tool for creating log file of every started and stopped process and applied traps</li> 		  
            <li>You can make configuration portable using INI file instead of registry base configuration.</li>
            <li>Take care: <a href="/docs/security/virus_trojan_malware_spyware.php" title="process name cloaking">process names may be eclipsed</a> by malicious software.</li>
            <li>Command line tool for accessing process list, and kill process by name, kill menu item, or computing MD5 and SHA1 hashes</li>
            <li>Internationalization: Spanish, Italian, German, French, Chinese, Swedish, Valencian, Polish, Russian, Portuguese, Japanese, Arabic and Greek support.</li>
            <li>Enhanced (experimental) multidesktop feature: use up to four windows desktops using WindowsKey + F5 to F8.</li>
          </ul>
        </div>
      </div>    
		</div>

