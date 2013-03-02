from django.shortcuts import render_to_response
def display_meta(request):
    values = request.META.items()
    values.sort()
    return render_to_response('meta.html', {"meta_list":values})
