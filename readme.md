# How to work with this repository: #

Know that this is NOT the main MWorks repository (https://github.com/mworks/mworks). We (as in the VNL) have created our own fork on 7/24/15.

## You will need: ##

* A GitHub account which is linked to the VisualNeuroscienceLaboratory account (specifically for the mworks team; talk to Romesh...hopefully he's still here when you're reading this!)

* A copy of GitHub desktop

* A code editor (XCode is a nice option for Mac)

* MWServer, MWClient, MWEditor installed

### How to add this repository to your desktop GitHub ###

Once you have GitHub desktop installed, simply click "Clone in Desktop" (right-side of page) on the webpage associated with the desired repository. You'll be given the chance to choose where to save this repository locally.

### How and when to sync your repository ###

If you've made local changes, you should see a list of changes under the "Changes" tab in the desktop GitHub program. Check all the changes that you'd like to commit.

Click "Commit to master", located at the bottom when on the "Changes" tab.

Finally click the "Sync" button at the top right of the GitHub desktop program.

### Are there possible concurrency issues? ###

By this, we mean issues with having the same file open in multiple places (e.g. editing locally AND directly online, or two separate users editing locally).

When you go to commit and changes have been made (& committed) elsewhere on a file that you're attemping to commit, you'll be warned that changes have been made. This will give you a chance to check out those changes and decide which version to keep/edit.

### How to undo a commit ##

In GitHub desktop, go to the "History" tab. Find the change you wish to undo ("revert"). Above the changes on the right, click the gear wheel (settings) and select "Revert this Commit". This leaves everything else in the branch untouched except the commit that you're reverting. If you instead choose "Roll back to this Commit", everything committed to the branch since this update will be reverted.

### How to use MWorks ###

For serious help, check out http://mworks-project.org. 

To get a very simple example, open the client (MWClient) and the server (MWServer). Client-side, click the red X button and hit connect (no need to mess with the defaults unless you know what you're doing). Click the folder and choose a new experiment. Navigate to examples->Examples->DriftingGratingDemo relative to wherever you have the mworks source code saved. The "DriftingGratingDemo.xml" file should work.

### How to write a plug-in ###

An MWorks plug-in will let you add functionality without altering the core source code (which requires the permission of those who run MWorks).

As far as writing one...we'll work on getting a nice guide/template that improves on the existing template.

### Terms ###

https://help.github.com/articles/github-glossary/
