# Create your views here.
from django.http import HttpResponse
from django.template.loader import get_template
from django.template import Context
from django.shortcuts import render_to_response
import datetime

def main_page(request):
    now = datetime.datetime.now()
    output='''
        <html>
         <head><title>%s</title></head>
         <body>
             <h1>%s</h1>
             <p>%s</p>
             <p>%s</p>
         </body>
        </html>
    ''' %(
        'Django Bookmarks',
        'Welcome to Django Bookmarks',
        'Where you can store and share bookmarks',
        now
    )
    return HttpResponse(output)

def hours_ahead(request,offset):
    try:
        offset = int(offset)
    except ValueError:
        raise Http404()
    dt = datetime.datetime.now() + datetime.timedelta(hours=offset)
    assert False
    html = '''
    <html>
        <body>In %s hours, it will be %s.</body>
    </html>
    ''' %(offset, dt)
    return HttpResponse(html)

def current_datetime(request):
    now = datetime.datetime.now()
    #t = get_template('ex.html')
    #html = t.render(Context({'current_date':now}))
    #return HttpResponse(html)
    return render_to_response('base_datetime.html', {'current_date':now})
def search_form(request):
    return render_to_response('search_form.html')
def search(request):
    if 'q' in request.GET:
        message = 'You searched for: %s' % request.GET['q']
    else:
        message = 'You submitted an empty form.'
    return HttpResponse(message)
