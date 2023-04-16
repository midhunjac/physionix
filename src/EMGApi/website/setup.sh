#!/usr/bin/env sh

# This script sets up the website project
base_dir="$(pwd)"
nginx_dir="/etc/nginx"
web_dir="/var/www/html"

# Copy the website files
echo 'Copying website files...'
cp $base_dir/nginx-sites-enabled-default $nginx_dir/sites-enabled/default
cp -r $base_dir/include $base_dir/index.html $web_dir
echo 'Website files copied.'
