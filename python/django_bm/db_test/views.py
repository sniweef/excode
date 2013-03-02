# Create your views here.
#print 'hello'
#assert False
from django.http import HttpResponse
from django.shortcuts import render_to_response, render
from db_test.models import *
from django.core.mail import send_mail
from contact.contact import ContactForm

def search_book(request):
    error = False
    if 'q' in request.GET:
        q = request.GET['q']
        if q and len(q) < 20:
            books = Book.objects.filter(title__contains=q)
            return render_to_response('search_results.html',
                {'books':books, 'query':q})
        else:
            error = True
#        return HttpResponse('Please submit a valid term!')
    return render_to_response('search_book.html',
                {'error':error})
def contact(request):
    errors = []
    if request.method == 'GET':
        if not request.GET.get('subject', ''):
            errors.append('Enter a subject')
        if request.GET.get('email') and '@' not in request.GET['email']:
            errors.append('Enter a valid e-mail address.')
        if not request.GET.get('message', ''):
            errors.append('Enter a message.')
        if not errors:
            send_mail(
                request.GET['subject'],
                request.GET['message'],
                request.GET.get('email', 'noreply@example.com'),
                ['hzhigeng@gmail.com'],
            )
            return HttpResponseRedict('contact/thanks')
    return render_to_response('contact_form.html', {'errors':errors})

def contact_use_Form(request):
    if request.method == 'POST':
        form = ContactForm(request.POST)
        if form.is_valid():
            cd = form.cleaned_data
            send_mail(
                      cd['subject'],
                      cd['message'],
                      cd.get('email', 'noreply@example.com'),
                      ['siteowner@example.com'],
            )
            return HttpResponseRedirect('/contact/thanks/')
        #return HttpResponse('sending mail...')
    else:
        form = ContactForm()
        #return render(request, 'contact_form2.html', {'form':form})
    return render_to_response('contact_form2.html', {'form':form})
