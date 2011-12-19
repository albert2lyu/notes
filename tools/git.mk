Git FQA
========

FQA
----

### Delete a remote Git tag ### 
If you have a tag named '12345' then you would just do this:
<pre><code> 
git tag -d 12345
git push origin :refs/tags/12345
</code></pre>

That will remove '12345' from the remote repository.

### Delete a remote branch in git ### 
The command to delete a remote branch “feature1″ is

<pre><code> 
git push origin :feature1
</code></pre>

Just wanted to note this down.
