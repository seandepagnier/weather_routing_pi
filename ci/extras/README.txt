This directory can contain 0, 1, or more files that can be used to install extra libraries/programs
that need to be installed to ensure the build will be sucessful. There are three basic file names:
    extra_libs.txt
    {script name}_extra_libs.txt
    {CIRCLE_JOB}_extra.libs.txt

The first will be executed by all scripts, the second will only be executed by a specific script.
The second form will look like:
    circleci-build-debian_extra_libs.txt
where 'circleci-build-debian' is the script name with the '.sh' removed.
The third form will look like:
    build-jammy_extra_libs.txt
where 'build-jammy' is the circleci job.

