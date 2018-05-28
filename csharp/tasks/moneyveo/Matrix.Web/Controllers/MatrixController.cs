using Matrix.Handlers;
using Matrix.Services;
using Matrix.Web.Helpers;
using System;
using System.Web;
using System.Web.Mvc;

namespace Matrix.Web.Controllers
{
    public class MatrixController : Controller
    {
        private readonly IMatrixStorage _matrixStorage;

        private readonly IMatrixSerializer _matrixSerializer;

        public MatrixController(IMatrixStorage matrixStorage, IMatrixSerializer matrixSerializer)
        {
            _matrixStorage = matrixStorage;
            _matrixSerializer = matrixSerializer;
        }

        /// <summary>
        /// Грид показывает матрицу
        /// </summary>
        [HttpGet]
        public ActionResult Index(Guid? id)
        {
            if (!id.HasValue)
                return RedirectToAction("Index", "Default");

            var matrix = _matrixStorage.Get<int>(id.Value);

            ViewBag.MatrixKey = id.Value;

            return View(matrix);
        }

        /// <summary>
        /// Сгенерировать рандомную матрицу;
        /// </summary>
        [HttpGet]
        public ActionResult Create()
        {
            var matrix = Matrix.Create<int>(GetMatrixSize);

            matrix.Handling(new MatrixRandomizer<int>(x => x.Next(10,99)));

            Guid id = _matrixStorage.Put<int>(matrix);

            return RedirectToAction("Index", new { id });
        }

        /// <summary>
        /// Повернуть матрицу на 90 градусов;
        /// </summary>
        [HttpGet]
        public ActionResult Rotate(Guid id)
        {
            var matrix = _matrixStorage.Get<int>(id);

            matrix.Handling(new MatrixRotation<int>());

            _matrixStorage.Put<int>(id, matrix);

            return RedirectToAction("Index", new { id });
        }

        /// <summary>
        /// Загрузить матрицу из файла .csv;
        /// </summary>
        [HttpPost]
        public ActionResult Import(HttpPostedFileBase file, string contentType)
        {
            var matrix = _matrixSerializer.Deserialize<int>(file.InputStream, contentType);

            Guid id = _matrixStorage.Put<int>(matrix);

            return RedirectToAction("Index", new { id });
        }

        /// <summary>
        /// Экспортировать матрицу в файл .csv;
        /// </summary>
        [HttpPost]
        public ActionResult Export(Guid id, string contentType)
        {
            var matrix = _matrixStorage.Get<int>(id);

            var filename = FileHelper.GetFileName(id, contentType);

            return File(_matrixSerializer.Serialize<int>(matrix, contentType), contentType, filename);
        }

        private static int GetMatrixSize
        {
            get
            {
                int max;

#if DEBUG
                max = 100;
#else
                unchecked
                {
                    max = (int)((GC.GetTotalMemory(true) / sizeof(int)) >> 8);
                }

                max = max == -1 ? int.MaxValue : max;
#endif
                return max;
            }
        }
    }
}