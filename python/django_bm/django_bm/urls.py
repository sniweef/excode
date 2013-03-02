from django.conf.urls import patterns, include, url
from bookmarks.views import *
from bookmarks.metas import *
from db_test.views import *

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'django_bm.views.home', name='home'),
    # url(r'^django_bm/', include('django_bm.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    (r'^admin/', include(admin.site.urls)),
    (r'^contact/$', contact),
    (r'^contact-use-form/$', contact_use_Form),
    (r'^date/$', current_datetime),
    (r'^index/$', main_page),
    (r'^meta/$', display_meta),
    (r'^search-form/$', search_form),
    (r'^search/$', search),
    (r'^search-book/$', search_book),
    (r'^time/plus/(\d{1,2})/$', hours_ahead),
)
