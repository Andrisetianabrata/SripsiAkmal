// Call the dataTables jQuery plugin
$(document).ready(function () {
  $('#dataTables').DataTable({
    order: [[0, 'desc']],
    layout: {
      bottomStart: {
        buttons: ['csv', 'excel', 'print']
      }
    }
  });
});
